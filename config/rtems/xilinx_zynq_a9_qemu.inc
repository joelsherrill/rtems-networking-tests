#
# Configuration for RTEMS Xilinx Zynq (for Qemu) build
#
# NOTE: There are variants of the Zynq BSP for various boards. When targeting
#       real hardware, use one of those.
#

# This source code is provided "AS IS" WITHOUT WARRANTY OF ANY KIND,
# EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
# NON-INFRINGEMENT. Some jurisdictions do not allow the exclusion of
# implied warranties, so the above exclusion may not apply to you.
#
# This work, authored by On-Line Applications Research Corporation, was
# funded by the U.S. Government under U.S. Government contract number 
# FA8075-14-D-0014. Therefore, the Government has unlimited rights.

RTEMS_ARCH=arm
RTEMS_BSP=xilinx_zynq_a9_qemu
RTEMS_VERSION=6

RTEMS_BASE=/home/joel/rtems-work/tools/$(RTEMS_VERSION)
RTEMS_TOOLS=$(RTEMS_BASE)/bin
RTEMS_BSPDIR=$(RTEMS_BASE)/bsp-install/$(RTEMS_ARCH)-rtems$(RTEMS_VERSION)/$(RTEMS_BSP)

ifeq ($(TCPIP_STACK),)
  TCPIP_STACK=libbsd
endif

RTEMS_CFLAGS=-B $(RTEMS_BSPDIR)/lib/ -specs bsp_specs -qrtems

TARGET_CPU_CFLAGS  = -march=armv7-a -mthumb -mfpu=neon
TARGET_CPU_CFLAGS += -mfloat-abi=hard -mtune=cortex-a9
TARGET_CPU_CFLAGS += -O0 -g
TARGET_CPU_CFLAGS += -ffunction-sections -fdata-sections

LDLIBS += -Wl,--gc-sections

ifeq ($(TCPIP_STACK),libbsd)
  LDLIBS += -lbsd
  LDLIBS += -lm
  LDLIBS += -ltelnetd

  TARGET_BSP_NETWORK_CONFIG_H = config/rtems/rtems_network_libbsd.h
endif


TARGET_OS_RUNNER=./config/rtems/runner_xilinx_zynq_a9_qemu.sh

include $(TOPDIR)/config/rtems/common.inc
