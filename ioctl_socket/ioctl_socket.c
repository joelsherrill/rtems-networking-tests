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
#include <sys/ioctl.h>

#ifdef SKIPPY
#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256
#endif  /*SKIPPY*/

int main(int argc, char *argv[])
{
  int      fd;
  int      rc;
  int      option;

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

  printf("*** Exercise ioctl ***\n");

  /* create what looks like an ordinary UDP socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
     perror("socket");
     exit(1);
  }

  /*
   * Test ioctl to set non-blocking
   */
  option = 1;
  rc = ioctl(fd, FIONBIO, &option);
  if (rc < 0) {
     perror("ioctl");
     exit(1);
  }
  fprintf(stderr, "ioctl() set non-blocking = (option=%d)\n", option);

  option = 0;
  rc = ioctl(fd, FIONBIO, &option);
  if (rc < 0) {
     perror("ioctl");
     exit(1);
  }
  fprintf(stderr, "ioctl() set blocking = (option=%d)\n", option);

  printf("*** End of exercise ioctl ***\n");
  sleep(5);
  exit(0);
}
