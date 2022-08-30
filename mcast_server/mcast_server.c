/*
 * ORIGIN:
 * http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/example.html
 */

/*
 * listener.c -- joins a multicast group and echoes all data it receives from
 *                the group to its stdout...
 *
 * Antony Courtney,        25/11/94
 * Modified by: Frédéric Bastien (25/03/04)
 * to compile without warning and work correctly
 */

#define _BSD_SOURCE       // to get multicast setsockopt() commands
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

/*jk*/
#define LWIP

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

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
  struct sockaddr_in addr;
  int             fd;
  ssize_t         nbytes;
  socklen_t       addrlen;
  struct ip_mreq  mreq;
  char            msgbuf[MSGBUFSIZE];
  int             yes;
  int             rc;
  char            *group;
  int              port;
  int              messages;

  yes = 1;

  if (argc != 3 ) {
    group = HELLO_GROUP;
    port = HELLO_PORT;
  } else {
    group = argv[1];
    port = atoi(argv[2]);
  }

  printf("Multicast listener on %s:%d\n", group, port);

  /* create what looks like an ordinary UDP socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
     error("socket");
  }

  /* allow multiple sockets to use the same IP address */
  rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if (rc < 0) {
    error("Reusing ADDR failed");
  }

#ifndef LWIP  /* lwip does not provide so_reuseport and it's not in FACE Safety Base */
  /* allow multiple sockets to use the same PORT number */
  rc = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
  if (rc < 0) {
    error("Reusing PORT failed");
  }
#endif /*LWIP*/

  /* set up destination address */
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY); /* N.B.: differs from sender */
  addr.sin_port = htons(port);

  /* bind to receive address */
  rc = bind(fd, (struct sockaddr *) &addr, sizeof(addr));
  if (rc < 0) {
    error("bind");
  }

  /* use setsockopt() to request that the kernel join a multicast group */
  mreq.imr_multiaddr.s_addr = inet_addr(group);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  rc = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
  if (rc < 0) {
    error("setsockopt IP_ADD_MEMBERSHIP");
  }

  /*
   * Exercise IP_MULTICAST_LOOP
   */
  int optval;
  int newval = 0xdeadbeef;
  socklen_t newlen = sizeof(newval);

  printf("set and get IP_MULTICAST_LOOP\n");
  newval = 0xdeadbeef;
  newlen = sizeof(newval);

  /* rc = getsockopt(fd, SOL_SOCKET, IP_MULTICAST_LOOP, &newval, &newlen); */
  rc = getsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &newval, &newlen);
  if (rc != 0 ) {
    error("getsockopt");
  }
  assert(newlen == sizeof(socklen_t));
  printf("getsockopt(IP_MULTICAST_LOOP) returned %d\n", newval);

  optval = (newval == 0);
  printf("setsockopt(IP_MULTICAST_LOOP, %d)\n", optval);
  /* rc = setsockopt(fd, SOL_SOCKET, IP_MULTICAST_LOOP, */
  rc = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP,
             (const void *)&optval, sizeof(int));
  if (rc != 0 ) {
    error("getsockopt");
  }

  newval = 0xdeadbeef;
  newlen = sizeof(newval);
  /* rc = getsockopt(fd, SOL_SOCKET, IP_MULTICAST_LOOP, &newval, &newlen); */
  rc = getsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &newval, &newlen);
  if (rc != 0 ) {
    error("getsockopt");
  }
  assert(newlen == sizeof(socklen_t));
  printf("getsockopt(IP_MULTICAST_LOOP) returned %d\n", newval);

  /* now just enter a read-print loop for 10 messages */
  fprintf(stderr, "Receiving 10 messages, then exiting\n");
  for (messages=0 ; messages <= 10 ; messages++ ) {
    addrlen = sizeof(addr);
    nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0,
                         (struct sockaddr *) &addr,&addrlen);
    if (nbytes < 0) {
      error("recvfrom");
    }
    fprintf(stderr, msgbuf);
  }

  /* use setsockopt() to request that the kernel join a multicast group */
  mreq.imr_multiaddr.s_addr = inet_addr(group);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  rc = setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
  if (rc < 0) {
    error("setsockopt IP_DROP_MEMBERSHIP");
  }

  rc = close(fd);
  if (rc < 0) {
    error("close");
  }

  sleep(5);
  exit(0);
}
