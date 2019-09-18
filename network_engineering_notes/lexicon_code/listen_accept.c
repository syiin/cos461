#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define MYPORT "3490"
#define BACKLOG 10 //how many listeners can wait in queue

/*
1. fill in the correct data structures:             getaddrinfo();
2. intialiise a socket:                             socket();
3. bind the initialised socket to a specific port:  bind();
4. listen for enquiries on the socket:              listen();
5. accept someone's connect() in the queue:         accept()

NB. accept() returns another socket descriptor so:
  socket1 - is still listening for new connections
  socket2 - to send() and recv()
*/

int main()
{
  struct sockaddr_storage their_addr; //we use this to be generic - either IPv4 or 6
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sockfd, new_fd;

  //set up the address structs we need
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AI_PASSIVE;

  getaddrinfo(NULL, MYPORT, &hints, &res);

  //setup the socket
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  bind(sockfd, res->ai_addr, res->ai_addrlen);
  listen(sockfd, BACKLOG);

  /*accept an incoming connection
  accept() points to a local sockaddr_storage and this is where information
  from the incoming connection will go - ie. you can determin which host:port
  info*/
  addr_size = sizeof their_addr;
  new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
  printf("LISTENING SOCK FD: %d\n", sockfd);
  printf("INC SOCK FD: %d\n", new_fd);
  return 0;
}