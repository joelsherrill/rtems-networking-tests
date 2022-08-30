/**
 * Origin:
 * http:// www.binarytides.com/multiple-socket-connections-fdset-select-linux/
 *
 * Handle multiple socket connections with select and fd_set on Linux
 */

#include <stdio.h>
#include <string.h>   // strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   // close
#include <arpa/inet.h>    // close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> // FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE   1
#define FALSE  0
#define PORT 8888

int main(int argc , char *argv[])
{
  int opt = TRUE;
  int master_socket;
  int addrlen;
  int new_socket;
  int client_socket[30];
  int max_clients = 30;
  int activity, i;
  int valread;
  int sd;
  int max_sd;
  int rc;
  struct sockaddr_in address;
  char buffer[1025];  // data buffer of 1K
  ssize_t message_len;
  ssize_t send_len;
  fd_set readfds; // set of socket descriptors

  // a message
  char *message = "ECHO Daemon v1.0 \r\n";

  // initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++) {
    client_socket[i] = 0;
  }

  // create a master socket
  master_socket = socket(AF_INET , SOCK_STREAM , 0);
  if (master_socket == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // set master socket to allow multiple connections,
  // this is just a good habit, it will work without this
  rc = setsockopt(
    master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
  if (rc < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // bind the socket to localhost port 8888
  rc = bind(master_socket, (struct sockaddr *)&address, sizeof(address));
  if (rc < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", PORT);

  // try to specify maximum of 3 pending connections for the master socket
  rc = listen(master_socket, 3);
  if (rc < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections ...");

  while(TRUE) {
    // clear the socket set
    FD_ZERO(&readfds);

    if (FD_ISSET(master_socket, &readfds)) {
      perror("FD_ZERO did not clear all");
      exit(EXIT_FAILURE);
    }

    // add master socket to set and exercise FD_CLR and FD_ISSET
    FD_SET(master_socket, &readfds);
    if (!FD_ISSET(master_socket, &readfds)) {
      perror("FD_SET did not set");
      exit(EXIT_FAILURE);
    }

    FD_CLR(master_socket, &readfds);
    if (FD_ISSET(master_socket, &readfds)) {
      perror("FD_CLR did not set");
      exit(EXIT_FAILURE);
    }

    FD_SET(master_socket, &readfds);

    max_sd = master_socket;

    // add child sockets to set
    for (i = 0 ; i < max_clients ; i++) {
      // socket descriptor
      sd = client_socket[i];

      // if valid socket descriptor then add to read list
      if (sd > 0)
        FD_SET(sd , &readfds);

      // highest file descriptor number, need it for the select function
      if (sd > max_sd)
        max_sd = sd;
    }

    // wait for an activity on one of the sockets.
    // Timeout is NULL, so wait indefinitely
    activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
    if ((activity < 0) && (errno!=EINTR))
    {
      printf("select error");
    }

    // If something happened on the master socket,
    // then its an incoming connection
    if (FD_ISSET(master_socket, &readfds)) {
      new_socket = accept(
        master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
      if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      // inform user of socket number - used in send and receive commands
      printf(
        "New connection, socket fd is %d, ip is : %s, port : %d \n",
        new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));


      // send new connection greeting message
      message_len = strlen(message);
      send_len = send(new_socket, message, strlen(message), 0) ;
      if (message_len != send_len) {
        perror("send");
      }

      puts("Welcome message sent successfully");

      // add new socket to array of sockets
      for (i = 0; i < max_clients; i++) {
        // if position is empty
        if (client_socket[i] == 0) {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n" , i);
          break;
        }
      }
    }

    // else its some IO operation on some other socket :)
    for (i = 0; i < max_clients; i++) {
      sd = client_socket[i];
      if (sd == 0) {
        continue;
      }

      if (FD_ISSET(sd , &readfds)) {
        // Check if it was for closing , and also read the incoming message
        if ((valread = read(sd , buffer, 1024)) == 0) {
          // Somebody disconnected , get his details and print
          getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
          printf("Host disconnected, ip %s, port %d \n",
            inet_ntoa(address.sin_addr), ntohs(address.sin_port));

          // Close the socket and mark as 0 in list for reuse
          close(sd);
          client_socket[i] = 0;
        } else {
          // Echo back the message that came in
          // set the string terminating NULL byte on the end of the data read
          buffer[valread] = '\0';
          message_len = strlen(buffer);
          send_len = send(sd , buffer , message_len , 0);
          if (send_len != message_len) {
            perror("send");
            exit(EXIT_FAILURE);
          }
        }
      }
    }
  }

  return 0;
}
