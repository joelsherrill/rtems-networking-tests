/*
 * ORIGIN:
 * http://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/
 */

/*
 *   Simple TCP client to fetch a web page
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int socket_desc;
  int rc;
  struct sockaddr_in server;
  char  message[128];
  char  server_reply[6000];
  ssize_t bytes;
  char *ip;
  char *port;
  char *host;

  host = "devel.rtems.org";
  ip = "140.211.10.146";
  port = "80";

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    perror("socket");
    exit(1);
  }

  // ip address of www.msn.com (get by doing a ping www.msn.com at terminal)
  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_family = AF_INET;
  server.sin_port = htons( atoi(port) );

  // Connect to remote server
  rc = connect(socket_desc, (struct sockaddr *)&server, sizeof(server));
  if (rc < 0)
  {
    perror("connect");
    exit(1);
  }

  // Send some data
  sprintf(message, "GET /?st=1 HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  bytes = send(socket_desc, message, strlen(message), 0);
  if (bytes < 0)
  {
    puts("Send failed");
    exit(1);
  }

  // Receive a reply from the server
  bytes = recv(socket_desc, server_reply, sizeof(server_reply), 0);
  if (bytes < 0)
  {
    perror("recv");
    exit(1);
  }
  puts(server_reply);
  sleep(5);
  exit(0);
}
