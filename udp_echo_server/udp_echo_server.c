/*
 * ORIGIN:
 * http://www.cs.rpi.edu/~goldsd/docs/spring2014-csci4220/echo-server-udp.c.txt
 */

/* UDP echo server program */

#include <stdio.h>      /* standard C i/o facilities */
#include <stdlib.h>     /* needed for atoi() */
#include <unistd.h>     /* defines STDIN_FILENO, system calls,etc */
#include <sys/types.h>  /* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h>  /* IP address conversion stuff */
#include <netdb.h>      /* gethostbyname */
#include <stdbool.h>
#include <string.h>

#define MAXBUF 256

static void echo( int sd ) {
  int received;
  socklen_t len;
  char bufin[MAXBUF];
  struct sockaddr_in remote;
  ssize_t sent;
  bool    stop;
  int     rc;

  len = sizeof(remote);

  stop = false;
  while (1) {
    received = recvfrom(sd,bufin,MAXBUF,0,(struct sockaddr *)&remote,&len);
    if (received<0) {
      perror("Error receiving data");
      return;
    }
    printf("GOT %d BYTES\n",received);
    sent = sendto(sd, bufin, received, 0,(struct sockaddr *)&remote, len);
    if (sent != received) {
      perror("Error sending data");
      return;
    }

    if (stop == true) {
      printf("Exiting UDP server\n");
      return;
    }

    /* clear remainder of buffer before printing */
    memset(&bufin[received], 0, sizeof(bufin)-received);
    printf("Buffer: %s\n", bufin);

    if (!strncmp(bufin, "recv\n", 5)) {
      printf("shutdown receiving\n");
      rc = shutdown(sd, SHUT_RD);
      if (rc < 0) {
        perror("shutdown recv");
      }
      stop = true;
      continue;
    } else if (!strncmp(bufin, "send\n", 5)) {
      printf("shutdown sending\n");
      rc = shutdown(sd, SHUT_WR);
      if (rc < 0) {
        perror("shutdown send");
      }
      stop = true;
      continue;
    } else if (!strncmp(bufin, "both\n", 5)) {
      printf("shutdown both sending receiving\n");
      rc = shutdown(sd, SHUT_RDWR);
      if (rc < 0) {
        perror("shutdown send");
      }
      stop = true;
      continue;
    }
  }
}

#define PORT 1501

int main(int argc, char **argv)
{
  int rc;
  int ld;
  struct sockaddr_in skaddr;
  socklen_t length;
  char hostname[128];

  if ((ld = socket( AF_INET, SOCK_DGRAM, 0 )) < 0) {
    printf("Problem creating socket\n");
    exit(1);
  }

  rc = gethostname(hostname, sizeof(hostname));
  if (rc != 0) {
    perror("gethostname");
    exit(1);
  }

  printf("\nUDP echo server on host/port %s/%d\n", hostname, PORT);

  skaddr.sin_family = AF_INET;
  skaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  // XXX want to pass socket of 0 and the need to call
  // XXX getsockname to know how to connect
  skaddr.sin_port = htons(PORT);
  if (bind(ld, (struct sockaddr *) &skaddr, sizeof(skaddr))<0) {
    printf("Problem binding\n");
    exit(0);
  }

  /* find out what port we were assigned and print it out */
  length = sizeof( skaddr );
  if (getsockname(ld, (struct sockaddr *) &skaddr, &length)<0) {
    printf("Error getsockname\n");
    exit(1);
  }

  /* port number's are network byte order, we have to convert to
     host byte order before printing !
  */
  printf("The server UDP port number is %d\n",ntohs(skaddr.sin_port));

  /* Go echo every datagram we get */
  printf("\nSend recv, send, or both to exercise shutdown()\n\n");
  echo(ld);
  return 0;
}
