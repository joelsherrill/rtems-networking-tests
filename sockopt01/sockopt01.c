/**
 * Exercise getsockopt() and setsockopt() for:
 *   - SO_REUSEADDR
 *   - SO_SNDBUF
 *   - SO_RCVBUF
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <assert.h>

/*
 * error - wrapper for perror
 */
static void error(char *msg)
{
  perror(msg);
  sleep(2);
  exit(1);
}

int sock;

/*
 * Exercise SO_REUSEADDR
 */
static void test_so_reuseaddr(void)
{
  int        rc;
  int        newval = 0xdeadbeef;
  socklen_t  newlen = sizeof(newval);
  int        optval; /* flag value for some setsockopt */

  printf("set and get SO_REUSEADDR\n");
  optval = 1;
  rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
             (const void *)&optval, sizeof(int));
  assert(rc == 0);

  rc = getsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
             &newval, &newlen);
  if (rc != 0 ) {
    perror("getsockopt");
    exit(0);
  }
  /* assert(newval == 1); */
  assert(newval != 0);  /* after 'set', lwip returns non-zero to indicate option-set, not 1 */
  assert(newlen == sizeof(socklen_t));
}

/*
 * Exercise SO_SNDBUF
 */
static void test_so_sndbuf(void)
{
  int        rc;
  int        newval = 0xdeadbeef;
  socklen_t  newlen = sizeof(newval);
  int        optval; /* flag value for some setsockopt */

  printf("set and get SO_SNDBUF\n");

  rc = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &newval, &newlen);
  if (rc != 0 ) {
    perror("getsockopt");
    exit(0);
  }
  assert(newlen == sizeof(socklen_t));
  printf("getsockopt(SO_SNDBUF) returned %d\n", newval);

  /* In lwip SNDBUF starts at maximum (which is also minimum) */
  optval = newval / 2;
  printf("setsockopt(SO_SNDBUF, %d)\n", optval);
  rc = setsockopt(sock, SOL_SOCKET, SO_SNDBUF,
             (const void *)&optval, sizeof(int));
  assert(rc == 0);

  newval = 0xdeadbeef;
  newlen = sizeof(newval);
  rc = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &newval, &newlen);
  if (rc != 0 ) {
    perror("getsockopt");
    exit(0);
  }
  assert(newlen == sizeof(socklen_t));
  printf("getsockopt(SO_SNDBUF) returned %d\n", newval);
}

/*
 * Exercise SO_RCVBUF
 */
static void test_so_rcvbuf(void)
{
  int        rc;
  int        newval = 0xdeadbeef;
  socklen_t  newlen = sizeof(newval);
  int        optval; /* flag value for some setsockopt */

  printf("set and get SO_RCVBUF\n");
  newval = 0xdeadbeef;
  newlen = sizeof(newval);

  rc = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &newval, &newlen);
  if (rc != 0 ) {
    perror("getsockopt");
    exit(0);
  }
  assert(newlen == sizeof(socklen_t));
  printf("getsockopt(SO_RCVBUF) returned %d\n", newval);

  /* optval = newval * 2; */
  optval = newval / 2;  /* default RCVBUF in lwip is int_max */
  printf("setsockopt(SO_RCVBUF, %d)\n", optval);
  rc = setsockopt(sock, SOL_SOCKET, SO_RCVBUF,
             (const void *)&optval, sizeof(int));
  assert(rc == 0);

  newval = 0xdeadbeef;
  newlen = sizeof(newval);
  rc = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &newval, &newlen);
  if (rc != 0 ) {
    perror("getsockopt");
    exit(0);
  }
  assert(newlen == sizeof(socklen_t));
  printf("getsockopt(SO_RCVBUF) returned %d\n", newval);
}

/*
 * Exercise SO_SNDTIMEO
 */
static void test_so_sndtimeo(void)
{
  int            rc;
  int            newval = 0xdeadbeef;
  socklen_t      newlen = sizeof(newval);
  struct timeval newtime;

  printf("set and get SO_SNDTIMEO\n");

  newtime.tv_sec = 0xdead;
  newtime.tv_usec = 0xfeed;
  newlen = sizeof(newtime);

  rc = getsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &newtime, &newlen);
  if (rc != 0 ) {
    perror("getsockopt SO_SNDTIMO");
    exit(0);
  }
  assert(newlen == sizeof(struct timeval));
  printf("getsockopt(SO_SNDTIMEO) returned %jd:%ld\n",
    (uintmax_t) newtime.tv_sec, newtime.tv_usec);

  newtime.tv_sec = 10;
  /* newtime.tv_usec = 15; */
  newtime.tv_usec = 7000;  /* lwip has millisec resolution for these timeouts */
  printf("setsockopt(SO_SNDTIMEO, %jd:%ld)\n",
    (uintmax_t) newtime.tv_sec, newtime.tv_usec);
  rc = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO,
             (const void *)&newtime, sizeof(struct timeval));
  if (rc != 0 ) {
    perror("setsockopt SO_SNDTIMEO");
    exit(0);
  }

  newtime.tv_sec = 0xdead;
  newtime.tv_usec = 0xfeed;
  newlen = sizeof(newtime);
  rc = getsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &newtime, &newlen);
  if (rc != 0 ) {
    perror("getsockopt SO_SNDTIMEO");
    exit(0);
  }
  assert(newlen == sizeof(struct timeval));
  printf("getsockopt(SO_SNDTIMEO) returned %jd:%ld\n",
    (uintmax_t) newtime.tv_sec, newtime.tv_usec);
}

/*
 * Exercise SO_RCVTIMEO
 */
static void test_so_rcvtimeo(void)
{
  int            rc;
  int            newval = 0xdeadbeef;
  socklen_t      newlen = sizeof(newval);
  struct timeval newtime;

  printf("set and get SO_RCVTIMEO\n");

  newtime.tv_sec = 0xdead;
  newtime.tv_usec = 0xfeed;
  newlen = sizeof(newtime);

  rc = getsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &newtime, &newlen);
  if (rc != 0 ) {
    perror("getsockopt SO_RCVTIMO");
    exit(0);
  }
  assert(newlen == sizeof(struct timeval));
  printf("getsockopt(SO_RCVTIMEO) returned %jd:%ld\n",
    (uintmax_t) newtime.tv_sec, newtime.tv_usec);

  newtime.tv_sec = 10;
  /* newtime.tv_usec = 15; */
  newtime.tv_usec = 3050;  /* lwip has millisec resolution for these timeouts */
  printf("setsockopt(SO_RCVTIMEO, %jd:%ld)\n",
    (uintmax_t) newtime.tv_sec, newtime.tv_usec);
  rc = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,
             (const void *)&newtime, sizeof(struct timeval));
  if (rc != 0 ) {
    perror("setsockopt SO_RCVTIMEO");
    exit(0);
  }

  newtime.tv_sec = 0xdead;
  newtime.tv_usec = 0xfeed;
  newlen = sizeof(newtime);
  rc = getsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &newtime, &newlen);
  if (rc != 0 ) {
    perror("getsockopt SO_RCVTIMEO");
    exit(0);
  }
  assert(newlen == sizeof(struct timeval));
  printf("getsockopt(SO_RCVTIMEO) returned %jd:%ld\n",
    (uintmax_t) newtime.tv_sec, newtime.tv_usec);
}

/*
 * Exercise TCP_NODELAY
 *   - fetch, toggle, and verify
 */
static void test_so_TCP_nodelay(void)
{
  int       rc;
  int       start_flags;
  int       flags;
  int       new_flags;
  socklen_t size = sizeof(flags);
  
  flags = 0xdead;
  rc = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, &size);
  if (rc != 0 ) {
    perror("getsockopt TCP_NODELAY");
    exit(0);
  }
  start_flags = flags;
  printf( "getsockopt(TCP_NODELAY) = %d\n", start_flags);

  flags = (start_flags) ? 0 : 1;
  rc = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, size);
  if (rc != 0 ) {
    perror("getsockopt TCP_NODELAY");
    exit(0);
  }
  printf( "setsockopt(TCP_NODELAY) = %d\n", flags);

  rc = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&new_flags, &size);
  if (rc != 0 ) {
    perror("getsockopt TCP_NODELAY");
    exit(0);
  }
  printf( "getsockopt(TCP_NODELAY) = %d\n", new_flags);
  if (new_flags != flags) {
    printf( "setsockopt did not take\n" );
    exit(0);
  }

}

int main(int argc, char **argv)
{
  printf("*** Exercise getsockopt/setsockopt ***\n");

  /*
   * socket: create the socket
   */
  printf("Create a socket\n");
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    error("ERROR opening socket");

  test_so_reuseaddr();
  test_so_sndbuf();
  test_so_rcvbuf();
  test_so_sndtimeo();
  test_so_rcvtimeo();
  test_so_TCP_nodelay();

  /*
   * Close and exit
   */
  close(sock);

  printf("*** End of exercise getsockopt/setsockopt ***\n");
  exit(0);
}
