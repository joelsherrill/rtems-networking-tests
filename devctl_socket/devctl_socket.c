/*
 * ORIGIN:
 * Written for purpose
 */

#define _POSIX_26_C_SOURCE

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
#include <sys/ioctl.h>
/*
 * RTEMS has posix_devctl. Otherwise, test using ioctl() to make sure
 * this test is sane.
 */
#ifdef __rtems__
  #include <devctl.h>
#else
  #define posix_devctl(_fd, _dcmd, _data1, _nbyte, _data2) \
    ioctl(_fd, _dcmd, _data1)
#endif

/*
 * error - wrapper for perror
 */
static void error(char *msg) {
  perror(msg);
  sleep(5);
  exit(1);
}

int main(int argc, char *argv[])
{
  int      fd;
  int      rc;
  int      option;


  puts("*** START OF posix_devctl socket TEST ***");

  /* create what looks like an ordinary UDP socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
     error("socket");
  }

  /*
   * Test posix_devctl to set non-blocking
   */
  option = 1;
  rc = posix_devctl(fd, FIONBIO, &option, sizeof(option), NULL);
  if (rc < 0) {
     error("posix_devctl");
  }
  fprintf(stderr, "posix_devctl() set non-blocking = (option=%d)\n", option);

  option = 0;
  rc = posix_devctl(fd, FIONBIO, &option, sizeof(option), NULL);
  if (rc < 0) {
     error("posix_devctl");
  }
  fprintf(stderr, "posix_devctl() set blocking = (option=%d)\n", option);

  puts("*** END OF posix_devctl socket TEST ***");
  sleep(5);
  exit(0);
}
