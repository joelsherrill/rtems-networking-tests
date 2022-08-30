/*
 * ORIGIN:
 * http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/example.html
 *
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,	25/11/94
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HELLO_GROUP "225.0.0.37"
#define HELLO_PORT 12345

int main(int argc, char *argv[])
{
  struct sockaddr_in  addr;
  int                 fd;
  char                message[128];
  ssize_t             sent;
  char               *group;
  int                 port;
  int                 count;
  size_t              length;

  if (argc != 3 ) {
    group = HELLO_GROUP;
    port = HELLO_PORT;
  } else {
    group = argv[1];
    port = atoi(argv[2]);
  }

  printf("Multicast writer on %s:%d\n", group, port);

  /* create what looks like an ordinary UDP socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    perror("socket");
    exit(1);
  }

  /* set up destination address */
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(group);
  addr.sin_port = htons(port);

  count = 1;
  /* now just sendto() our destination! */
  while (1) {
    sprintf(message, "This is message %d\n", count++);
    length = strnlen(message, sizeof(message)) + 1;
    fprintf(stderr, message);
    sent = sendto(fd, message, length, 0, (struct sockaddr *) &addr,
                  sizeof(addr));
    if (sent < 0) {
      perror("sendto");
      exit(1);
    }
    sleep(1);
  }
}
