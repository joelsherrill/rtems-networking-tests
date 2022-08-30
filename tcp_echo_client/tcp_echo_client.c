/*
 * ORIGIN:
 * https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server
 *
 * With modifications
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * error - wrapper for perror
 */
static void error(char *msg) {
  perror(msg);
  sleep(5);
  exit(1);
}

int main(int argc, char **argv)
{
  int sockfd;
  int rc;
  char sendline[100];
  char recvline[100];
  struct sockaddr_in servaddr;
  ssize_t to_write;
  ssize_t bytes;
  char dest[64];
  const char *p;
  char *ip = "127.0.0.1";
  char *port = "22001";

  if (argc == 3) {
    ip = argv[1];
    port = argv[2];
  }

  printf("Connecting to %s/%s\n", ip, port);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("socket");
  }

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(port));

  rc = inet_pton(AF_INET, ip, &servaddr.sin_addr);
  if (rc < 0) {
    error("inet_pton");
  }

  p = inet_ntop(AF_INET, &servaddr.sin_addr, dest, sizeof(dest));
  if (p == NULL) {
    error("inet_ntop");
  }

  if (strncmp(ip, dest, sizeof(dest))) {
    printf("start: %s end: %s\n", ip, dest);
    exit(0);
  }

  // now connect
  rc = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (rc < 0) {
    error("connect");
  }

  while(1) {
    memset(sendline, 0, sizeof(sendline));
    memset(recvline, 0, sizeof(recvline));
    fgets(sendline, sizeof(sendline), stdin);

    to_write = strlen(sendline) + 1;
    bytes = write(sockfd, sendline, to_write);
    if (bytes != to_write) {
      error("write");
    }

    bytes = read(sockfd, recvline, sizeof(recvline));
    printf("%zd: %s", bytes, recvline);
  }

  sleep(5);
  exit(0);
}
