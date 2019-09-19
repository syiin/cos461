#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

/*
A SIMPLE STREAM SERVER THAT ACCEPTS CONNECTIONS AND REPLIES WITH "HELLO WORLD" OVER
A STREAM CONNECTION
*/

#define PORT "3490" //which port users are connecting to
#define BACKLOG 10  //how many pending connections in queue

void sigchld_handler(int s)
{
  int saved_errno = errno;
  //waitpid() waits for a child process to to end
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
  errno = saved_errno;
}

//get socket address whether IPv4 or 6
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &((struct sockaddr_in *)sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *)sa)->sin6_addr;
}

int main()
{
  int sockfd, new_fd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; //connector's addr info
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     //IPv4 or 6
  hints.ai_socktype = SOCK_STREAM; //A stream socket - eg. TCP
  hints.ai_flags = AI_PASSIVE;

  //check that we can initialise the address info structs properly
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  //INITIALISE THE SERVER - eg. a host can have multiple IP addresses
  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    //try to initialise socket - go to the next available address if unable
    if ((sockfd = socket(p->ai_family,
                         p->ai_socktype,
                         p->ai_protocol)) == -1)
    {
      perror("server: socket"); //prints descriptive error messagge
      continue;
    }
    //try to set socket option name
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      perror("setsockopt");
      exit(1);
    }

    //try to bind, or else close opened socket and try on new addr info
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo);

  if (listen(sockfd, BACKLOG) == -1)
  {
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; //sigaction handler - reap all dead processes
  sigemptyset(&sa.sa_mask);        //clear all signals from set
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1)
  {
    perror("seigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");
  //LISTEN FOR INCOMING CONNECTION
  while (1)
  {
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1)
    {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family,
              get_in_addr((struct sockaddr *)&their_addr),
              s, sizeof s);
    printf("server: got connection from %s\n", s);

    if (!fork()) //this is the child process
    {
      close(sockfd);
      if (send(new_fd, "Hello World!", 13, 0) == -1)
      {
        perror("send");
      }
      close(new_fd);
      exit(0);
    }
    close(new_fd); //parent doesn't need this
  }

  return 0;
}