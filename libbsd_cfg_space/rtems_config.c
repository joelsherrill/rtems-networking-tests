
#define _BSD_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#include <rtems/shell.h>

#include "network_init.h"

static int sendspace;
static int recvspace;

static int sysctl_get(
  const char *name
)
{
  int     value;
  size_t  len;
  int     rc;

  len = sizeof(int);
  rc = sysctlbyname( name, &value, &len, NULL, 0);
  if (rc != 0) {
    perror("sysctlbyname: get");
    return -1;
  }
  // printf("%s=%d\n", name, value);
  return value;
}

static int sysctl_set(
  const char *name,
  int         newvalue
)
{
  size_t  len;
  int     rc;

  len = sizeof(int);
  rc = sysctlbyname(name, NULL, 0U, &newvalue, len);
  if (rc != 0) {
    perror("sysctlbyname: set");
    return -1;
  }
  printf("%s => %d\n", name, newvalue);
  return newvalue;
}

static void fetch_values(
  const char *message
)
{
  puts(message);

  recvspace = sysctl_get("net.inet.tcp.recvspace");
  printf("net.inet.tcp.recvspace = %d\n", recvspace);
  if (recvspace < 0) {
    exit(-1);
  }

  sendspace = sysctl_get("net.inet.tcp.sendspace");
  printf("net.inet.tcp.sendspace = %d\n", sendspace);
  if (sendspace < 0) {
    exit(-1);
  }
}

static void set_values(
  const char *message,
  int         recv_size,
  int         send_size
)
{
  puts(message);

  recvspace = sysctl_set("net.inet.tcp.recvspace", recv_size);
  if (recvspace < 0) {
    exit(-1);
  }

  sendspace = sysctl_set("net.inet.tcp.sendspace", send_size);
  if (sendspace < 0) {
    exit(-1);
  }
}

static void *POSIX_Init(void *arg)
{
  int rc;

  (void) arg;  /* deliberately ignored */

  puts("*** START OF LIBBSD CFG SEND/RECV SPACE ***");

  /*
   * Place some contents in the RAM file system before libsbd
   * is initialized so it can read configuration values.
   *
   * NOTE: /etc/sysctl.conf is currently not supported by libbsd.
   */

  /*
   * Initialize optional services
   */
  rc = initialize_network();
  assert(rc == 0);

  fetch_values("Default values:");
  set_values("Set values", 128 * 1024, 64 * 1024);

  fetch_values("Verify values were set");

  puts("*** END OF LIBBSD CFG SEND/RECV SPACE ***");
  exit(rc);

  return NULL;
}

#include <bsp.h> /* for device driver prototypes */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MICROSECONDS_PER_TICK     1000

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 32

#define CONFIGURE_MINIMUM_TASK_STACK_SIZE (64 * 1024)

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
