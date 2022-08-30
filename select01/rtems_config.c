
#include <assert.h>
#include <stdlib.h>

#include "network_init.h"

int main(int argc, char **argv);

static void *POSIX_Init(void *arg)
{
  int rc;

  (void) arg;  /* deliberately ignored */

  /*
   * Initialize optional services
   */
  rc = initialize_network();
  assert(rc == 0);

  /*
   * Could get arguments from command line or have a static set.
   */
  (void) main(0, NULL);

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
