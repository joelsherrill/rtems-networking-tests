# RTEMS Networking Tests

Networking tests for RTEMS that are intended to be TCP/IP stack independent. 

This source package contains a variety of network tests. Some are based
on examples from the Internet. Those have the origin noted in the file
comment block and include whatever license information was available.
Those tests will have an rtems_config.c to enable running them on RTEMS.
That file will be BSD-2.

Other tests in their entirety were developed for RTEMS and they are BSD-2.

Look at a specific file to determine the license.

## Building

This collection is currently built using 'make'. For GNU/Linux, 'make' and
'make clean' should be sufficient.

For RTEMS, you will need to add a config/rtems/*.ini file that corresponds 
to the BSP you want to build for. Building is similar to Linux except you
specify the .inc file to use:

make CONFIG_INC=config/rtems/xilinx_zynq_a9_qemu.inc

make CONFIG_INC=config/rtems/xilinx_zynq_a9_qemu.inc clean

## TODO

Improve the mechanisms for specifying and initializing each network stack.  


