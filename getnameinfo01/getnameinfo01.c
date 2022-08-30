/*
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

static
void initSockaddr(struct sockaddr_in* sa, const char* addrStr, uint16_t port) {
  sa->sin_family = AF_INET;
  sa->sin_port = htons(port);
  inet_pton(AF_INET, addrStr, &(sa->sin_addr));
}

static
void doGetnameinfo(const char* addrStr, uint16_t port, char* host, int hostlen, char* service, int servlen, char** outp) {
  struct sockaddr_in sa;
  if (host && (hostlen > 0)) {
    host[0] = '\0';
  }
  if (service && (servlen > 0)) {
    service[0] = '\0';
  }
  initSockaddr(&sa, addrStr, port);
  int rc = getnameinfo((const struct sockaddr*)&sa, sizeof sa, host, hostlen, service, servlen, 0);
  if (rc != 0) {
    int nc = sprintf(*outp, "getnameinfo(%s,%d) rc=%d\n", addrStr, port, rc);
    *outp += nc;
  } else {
    if (!service) {
      service = "(NULL)";
    }
    if (!host) {
      host = "(NULL)";
    }
    int nc = sprintf(*outp, "getnameinfo('%s',%d) host='%s' service=%s\n", addrStr, port, host, service);
    *outp += nc;
  }
}

static
const char* getnameinfo_test(void) {
  static char obuf[512];
  char host[1024];
  char service[20];

  char* outp = obuf;
  int nc;

  doGetnameinfo( "127.0.0.1", 7, host, sizeof(host), service, sizeof(service), &outp);

  doGetnameinfo("23.195.69.106", 80, host, sizeof(host), service, sizeof(service), &outp);  //www.example.com

  nc = sprintf(outp, "null service buffer:\n");
  outp += nc;
  doGetnameinfo( "127.0.0.1", 7, host, sizeof(host), NULL, 0, &outp);

  nc = sprintf(outp, "null host buffer:\n");
  outp += nc;
  doGetnameinfo( "127.0.0.1", 7, NULL, 0, service, sizeof(service), &outp);

  return obuf;
}

int main(int argc, char *argv[])
{
  sleep(1);
  puts("*** getnameinfo test ***\n");
  const char* outbuf = getnameinfo_test();
  puts(outbuf);
  puts("*** getnameinfo test done ***\n");
  sleep(5);
  exit(0);
}
