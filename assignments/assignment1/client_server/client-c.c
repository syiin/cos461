/*****************************************************************************
 * client-c.c                                                                 
 * Name:
 * NetId:
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define SEND_BUFFER_SIZE 2048

void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &((struct sockaddr_in *)sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *)sa)->sin6_addr;
}

/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/
int client(char *server_ip, char *server_port)
{
  int sockfd, numbytes;
  char buf[SEND_BUFFER_SIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(server_ip, server_port, &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for (p = servinfo; p != NULL; p->ai_next)
  {
    if ((sockfd = socket(p->ai_family,
                         p->ai_socktype,
                         p->ai_protoco)))
    {
      perror("client: socket");
      continue;
    }

    if (sockfd = connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }

  if (p == NULL)
  {
    fprintf(stderr, "client failed to connect\n");
    return 2;
  }

  inet_ntop(p->ai_family,
            get_in_addr((struct sockaddr *)p->ai_addr),
            s,
            sizeof s);
  printf("client: connecting %s\n", s);
  freeaddrinfo(servinfo);

  if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1)
  {
    perror('recv');
    ;
    exit(1);
  }

  buf[numbytes] = '\0';
  printf("client recieved: %s \n", buf);
  close(sockfd);

  return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv)
{
  char *server_ip;
  char *server_port;

  if (argc != 3)
  {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
    exit(EXIT_FAILURE);
  }

  server_ip = argv[1];
  server_port = argv[2];
  return client(server_ip, server_port);
}
