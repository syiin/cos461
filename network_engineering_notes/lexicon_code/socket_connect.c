#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
  //this opens a socket connection to www.example.com:3490
  struct addrinfo hints, *res;
  int sockfd, status;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("www.example.com", "3490", &hints, &res);
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  //(sock file descript, socket addr includes IP:Port & protocol info, length of socket addr)
  status = connect(sockfd, res->ai_addr, res->ai_addrlen);
  printf("%d\n", status);

  return 0;
}