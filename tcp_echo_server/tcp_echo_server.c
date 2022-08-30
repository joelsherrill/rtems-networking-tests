/*
 * Origin:
 * https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/tcpserver.c
 *
 * tcpserver.c - A simple TCP echo server
 * usage: tcpserver <number of times to echo> <port>
 */

#define _BSD_SOURCE // for gethostbyaddr from netdb.h
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
static void error(char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char **argv)
{
  int parentfd;                  /* parent socket */
  int childfd = -1;              /* child socket */
  int portno;                    /* port to listen on */
  int times_to_echo;             /* number of times to echo */
  socklen_t clientlen;           /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp;         /* client host info */
  char *hostaddrp;               /* dotted decimal host addr string */
  char buf[BUFSIZE];             /* message buffer */
  int optval;                    /* flag value for setsockopt */
  int n;                         /* message byte size */
  int rc;

  puts("*** START OF TCP ECHO SERVER ***");

  /*
   * check command line arguments
   */
  if (argc != 3) {
    fprintf(stderr, "usage: %s <times to echo> <port>\n", argv[0]);
    exit(1);
  }
  times_to_echo = atoi(argv[1]);
  portno = atoi(argv[2]);

  /*
   * socket: create the parent socket
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0)
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets
   * us rerun the server immediately after we kill it;
   * otherwise we have to wait about 20 secs.
   * Eliminates "ERROR on binding: Address already in use" error.
   */
  optval = 1;
  rc = setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
             (const void *)&optval, sizeof(optval));
  if (rc != 0 ) {
    perror("setsockopt");
    exit(0);
  }

  /* disabled because returns -1/EINVAL on Linux */
  int newval = 0xdeadbeef;
  socklen_t newlen = sizeof(newval);

  rc = getsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, &newval, &newlen);
  if (rc != 0 ) {
    perror("getsockopt");
    exit(0);
  }
  /* assert(newval == 1); */
  assert(newval != 0);  /* lwip returns non-zero to indicate option-is-set, not 1 */
  assert(newlen == sizeof(socklen_t));

  /*
   * build the server's Internet address
   */
  memset(&serveraddr, 0, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  /*
   * bind: associate the parent socket with a port
   */
  if (bind(parentfd, (struct sockaddr *) &serveraddr,
           sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  /*
   * listen: make this socket ready to accept connection requests
   */
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */
    error("ERROR on listen");

  /*
   * accept: wait for a connection request
   */
  clientlen = sizeof(clientaddr);
  childfd = accept(
    parentfd,
    (struct sockaddr *) &clientaddr,
    &clientlen
  );
  if (childfd < 0)
    error("ERROR on accept");

  /*
   * gethostbyaddr: determine who sent the message
   */
  hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
			sizeof(clientaddr.sin_addr.s_addr), AF_INET);
  if (hostp == NULL)
    error("ERROR on gethostbyaddr");
  hostaddrp = inet_ntoa(clientaddr.sin_addr);
  if (hostaddrp == NULL)
    error("ERROR on inet_ntoa\n");
  printf("server established connection with %s (%s)\n",
	 hostp->h_name, hostaddrp);

  /*
   * main loop: wait for a connection request, echo input line,
   * then close connection.
   */
  printf("Echoing %d times\n", times_to_echo);
  while (times_to_echo--) {

    /*
     * read: read input string from the client
     */
    memset(buf, 0, BUFSIZE);
    n = read(childfd, buf, BUFSIZE);
    if (n < 0)
      error("ERROR reading from socket");
    printf("server received %d bytes: %s", n, buf);

    /*
     * write: echo the input string back to the client
     */
    n = write(childfd, buf, strlen(buf));
    if (n < 0)
      error("ERROR writing to socket");
  }
  puts("Closing socket");
  close(childfd);
  puts("*** END OF TCP ECHO SERVER ***");

  return 0;
}
