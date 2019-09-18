#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
  struct addrinfo hints, *res;
  int sockfd, status;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; //use either IPv4 or 6, whichever
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; //fill in the IP for me - ie. bind to host

  getaddrinfo(NULL, "3490", &hints, &res); //remember &res comes back through the args

  //make a socket - socket returns a socket descriptor to use for later function calls
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  //bind the socket to the port passed in earlier
  status = bind(sockfd, res->ai_addr, res->ai_addrlen);
  printf("SOCKETS STATUS: %d\n", sockfd);
  printf("BIND STATUS: %d\n", status);
  return 0;
}