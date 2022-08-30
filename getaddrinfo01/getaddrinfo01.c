/*
 * ORIGIN:
 * http://www.logix.cz/michal/devel/various/getaddrinfo.c.xp
 *
 * getaddrinfo.c - Simple example of using getaddrinfo(3) function.
 *
 * Michal Ludvig <michal@logix.cz> (c) 2002, 2003
 * http://www.logix.cz/michal/devel/
 *
 * License: public domain.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

static int lookup_host (const char *host)
{
  struct addrinfo hints, *res;
  int errcode;
  char addrstr[100];
  void *ptr;

  ptr = NULL;
  memset (&hints, 0, sizeof (hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags |= AI_CANONNAME;

  errcode = getaddrinfo (host, NULL, &hints, &res);
  if (errcode != 0) {
    perror ("getaddrinfo");
    return -1;
  }

  printf ("Host: %s\n", host);
  while (res) {
    inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

    switch (res->ai_family) {
      case AF_INET:
        ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
        break;
      case AF_INET6:
        ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
        break;
    }
    inet_ntop (res->ai_family, ptr, addrstr, 100);
    printf ("IPv%d address: %s (%s)\n", res->ai_family == AF_INET6 ? 6 : 4,
            addrstr, res->ai_canonname);
    res = res->ai_next;
  }

  freeaddrinfo(res);
  puts(" *** end getaddrinfo test ***\n");
  sleep(5);
  return 0;
}

int main(int argc, char *argv[])
{
  int i;
  int rc;

  if (argc < 2) {
    printf("no hosts to look up\n");
    exit (1);
  }

  for (i=1 ; i<argc ; i++) {
    printf("Lookup host: %s\n", argv[i]);
    rc = lookup_host (argv[i]);
    if (rc != 0) {
      printf("Failed lookup for host: %s\n", argv[i]);
    }
  }

  exit(0);
}
