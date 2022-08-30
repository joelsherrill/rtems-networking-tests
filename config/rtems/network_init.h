/*
 * network_init.h
 * Description:  This header demonstrates minimally bringing the RTEMS libbsd
 * network stack.  This also includes the shell configuration items since libbsd
 * has it's own network shell commands.
 *
 */
#pragma once
#include <rtems/bsd/bsd.h>
#include <rtems/bsd/modules.h>
#include <machine/rtems-bsd-commands.h>
#include <rtems/dhcpcd.h>
#include <rtems/console.h>
#include <rtems/netcmds-config.h>
#include <bsp/irq-info.h>
#include <net/if.h>
#include <rtems/bsd/bsd.h>
#include <sysexits.h>

int initialize_network(void);

int initialize_network(void)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;
  int retval = 0;
  char ifnamebuf[IF_NAMESIZE];
  char *ifname;

  /* Initialize BSD library */
  sc = rtems_bsd_initialize();
  if (sc != RTEMS_SUCCESSFUL)
  {
    fprintf(stderr,
      "Initialization of RTEMS LibBSD Failed.  "
      "Status is %d: %s\n",
      sc, rtems_status_text(sc));
    return sc;
  }

  /* Let the callout timer allocate its resources */
  sc = rtems_task_wake_after(2);
  if (sc != RTEMS_SUCCESSFUL)
  {
    fprintf(stderr,
      "Failed to set the task wake timer.  "
      "Status is %d: %s\n",
      sc,
      rtems_status_text(sc)
    );
    return -1;
  }

  /* Bring up the loopback interface */
  retval = rtems_bsd_ifconfig_lo0();
  if (retval != EX_OK)
  {
    fprintf(stderr,
      "Failed to bring up the loopback interface.  Status is %d\n",
      retval
    );
    return -1;
  }

  /* setup the network adapter */
  ifname = if_indextoname(1, ifnamebuf);
  fprintf(stdout, "Found first Ethernet interface called %s\n", ifname);

  char *ifconfig_command[] = 
  {
    "ifconfig",
    ifname,
    "up",
    NULL
  };
  
  /* Bring up the specified ethenet interface */
  retval = rtems_bsd_command_ifconfig(
    RTEMS_BSD_ARGC(ifconfig_command), 
    ifconfig_command
  );
  if (retval != EX_OK)
  {
    fprintf(stderr,
      "Failed to bring up if %s with status code %d\n",
      ifname,
      retval);
    return -1;
  }

  /* Start the DHCP client */
  sc = rtems_dhcpcd_start(NULL);
  if (sc != RTEMS_SUCCESSFUL)
  {
    fprintf(stderr,
      "Failed to start DHCP client.  Status message is %s\n",
      rtems_status_text(sc)
    );
    return -1;       
  }

  return 0;
}

#define RTEMS_BSD_CONFIG_NET_PF_UNIX
#define RTEMS_BSD_CONFIG_NET_IP_MROUTE
#define RTEMS_BSD_CONFIG_NET_IF_BRIDGE
#define RTEMS_BSD_CONFIG_NET_IF_LAGG
#define RTEMS_BSD_CONFIG_NET_IF_VLAN
#define RTEMS_BSD_CONFIG_BSP_CONFIG
#define RTEMS_BSD_CONFIG_INIT

#include <machine/rtems-bsd-config.h>

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 32
#define CONFIGURE_UNLIMITED_ALLOCATION_SIZE 32

#define CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_BDBUF_BUFFER_MAX_SIZE (64 * 1024)
#define CONFIGURE_BDBUF_MAX_READ_AHEAD_BLOCKS 4
#define CONFIGURE_BDBUF_CACHE_MEMORY_SIZE (1 * 1024 * 1024)

#define CONFIGURE_MAXIMUM_DRIVERS 32

#define CONFIGURE_SHELL_USER_COMMANDS \
  &bsp_interrupt_shell_command, \
  &rtems_shell_ARP_Command, \
  &rtems_shell_HOSTNAME_Command, \
  &rtems_shell_PING_Command, \
  &rtems_shell_ROUTE_Command, \
  &rtems_shell_NETSTAT_Command, \
  &rtems_shell_IFCONFIG_Command, \
  &rtems_shell_TCPDUMP_Command, \
  &rtems_shell_SYSCTL_Command, \
  &rtems_shell_VMSTAT_Command

