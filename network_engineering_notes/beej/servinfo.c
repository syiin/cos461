#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  if (argc != 2)
  {
    fprintf(stderr, "usage: showip hostname\n");
    return 1;
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; //AF_INET or AF_INET6 to force version
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
  {
    //gai_strerror transforms error codes to human readable string
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
  }

  printf("IP addresses for %s: \n\n", argv[1]);

  for (p = res; p != NULL; p = p->ai_next)
  {
    void *addr;
    char *ipver;
    //IPv4
    if (p->ai_next == AF_INET)
    { //ie. ai_next == adddress info next, get pointer to address itself
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }
    else
    {
      //1. get the address and cast to ipv6 socket address struct
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      //2. dereference ipv6 struct address pointer
      addr = &(ipv6->sin6_addr);
      //3. set the ip version
      ipver = "IPv6";
    }

    //convert the IP to a string and print
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf(" %s: %s \n", ipver, ipstr);
  }

  freeaddrinfo(res); //free the linked list
  return 0;
}