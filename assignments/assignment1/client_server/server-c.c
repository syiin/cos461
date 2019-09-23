/*****************************************************************************
 * server-c.c                                                                 
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

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &((struct sockaddr_in *)sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *)sa)->sin6_addr;
}

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
*/
int server(char *server_port)
{
  int sockfd, client_fd, numbytes;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage client_addr;
  socklen_t sin_size;
  char s[INET6_ADDRSTRLEN];
  char buf[RECV_BUFFER_SIZE];
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if ((rv = getaddrinfo(NULL, server_port, &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "couldn't get addrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    //initialize socket
    if ((sockfd = socket(p->ai_family,
                         p->ai_socktype,
                         p->ai_protocol)) == -1)
    {
      perror("server: socket");
      continue;
    }

    //bind socket to my address
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }
  freeaddrinfo(servinfo);

  //listen to socket file
  if (listen(sockfd, QUEUE_LENGTH) == -1)
  {
    perror("listen");
    exit(1);
  }

  printf("server: waiting for connections...\n");
  while (1)
  {
    sin_size = sizeof client_addr;
    //accept an request from the listening socket and pass it to a new
    //client socket that handles messages from it
    client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
    if (client_fd == -1)
    {
      perror("accept");
      continue;
    }
    //converts address into a presentable format
    inet_ntop(client_addr.ss_family,
              get_in_addr((struct sockaddr *)&client_addr),
              s, sizeof s);
    printf("server: got connection from %s\n", s);

    //receive data from the client socket descriptor and save the message
    //to *buf
    if ((numbytes = recv(client_fd, buf, RECV_BUFFER_SIZE - 1, 0)) == -1)
    {
      perror("recv");
      exit(1);
    }

    buf[numbytes] = '\0';
    printf("server received: %s \n", buf);
    if (send(client_fd, "Message received", 17, 0 == -1))
    {
      perror("send");
    }
  }

  return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv)
{
  char *server_port;

  if (argc != 2)
  {
    fprintf(stderr, "Usage: ./server-c [server port]\n");
    exit(EXIT_FAILURE);
  }

  server_port = argv[1];
  return server(server_port);
}
