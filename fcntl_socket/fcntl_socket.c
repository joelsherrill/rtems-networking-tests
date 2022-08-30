/*
 * ORIGIN:
 * Written for purpose
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
#include <assert.h>
#include <fcntl.h>

#ifdef SKIPPY
#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256
#endif /*SKIPPY*/

int main(int argc, char *argv[])
{
  int      fd;
  int      flags;

#ifdef SKIPPY
  char    *group;
  int      port;

  if (argc != 3 ) {
    group = HELLO_GROUP;
    port = HELLO_PORT;
  } else {
    group = argv[1];
    port = atoi(argv[2]);
  }

  printf("Multicast listener on %s:%d\n", group, port);
#endif /*SKIPPY*/

  printf("*** Exercise fcntl ***\n");


  /* create what looks like an ordinary UDP socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
     perror("socket");
     exit(1);
  }

  /*
   * Test fcntl to set non-blocking
   */
  flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
     perror("fcntl");
     exit(1);
  }
  fprintf(stderr, "fcntl(F_GETFL) non-blocking = %d\n",
    ((flags & O_NONBLOCK) == O_NONBLOCK));


  fprintf(stderr, "fcntl(F_SETFL) set non-blocking\n");
  flags |= O_NONBLOCK;
  flags = fcntl(fd, F_SETFL, flags);
  if (flags < 0) {
     perror("fcntl");
     exit(1);
  }

  flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
     perror("fcntl");
     exit(1);
  }
  fprintf(stderr, "fcntl(F_GETFL) non-blocking = %d\n",
    ((flags & O_NONBLOCK) == O_NONBLOCK));

  printf("*** End of exercise fcntl ***\n");
  sleep(5);
  exit(0);
}
