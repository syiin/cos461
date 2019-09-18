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

//sample call to listen to host IP address at socket 3490
//this doesn't actually connect - it just setups the structures we need
int main()
{
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;

  //check the structs are empty - memset fills a block of memory with
  //a particular value eg. 0
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     //don't care IP version
  hints.ai_socktype = SOCK_STREAM; //TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     //fill in my IP for me

  status = getaddrinfo(NULL, "3490", &hints, &servinfo);
  //servinfo now points to a linked list of struct addrinfos
  printf("%d\n", status);
  return 0;
}