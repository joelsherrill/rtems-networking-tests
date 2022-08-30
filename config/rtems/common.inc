#
# Configuration with shared RTEMS settings expected to be the same
# across all architectures and BSPS
#

# The following is set for the extension on any executables built
EXEEXT=.exe

# RTEMS 5 still had bsp_specs
ifeq ($(RTEMS_VERSION),5)
  RTEMS_CFLAGS=-B $(RTEMS_BSPDIR)/lib/ -specs bsp_specs -qrtems
else
#endif
#ifneq ($(RTEMS_VERSION),5)
  RTEMS_CFLAGS=-B $(RTEMS_BSPDIR)/lib/ -qrtems
endif

# Does the OS provide posix_devctl()? If no, it is provided by FIA
OS_Provides_posix_devctl=yes

CC      = $(RTEMS_TOOLS)/$(RTEMS_ARCH)-rtems$(RTEMS_VERSION)-gcc

CFLAGS += $(RTEMS_CFLAGS)
CFLAGS += $(TARGET_CPU_CFLAGS)
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -std=c99
CFLAGS +=  -pthread
CFLAGS += -Wno-nonnull-compare

# To see POSIX source that is beyond C standard since we use -std=c99
CPPFLAGS += -D_POSIX_C_SOURCE=200809L

CPPFLAGS += -I$(TOPDIR)/config/rtems/

LDFLAGS  = $(RTEMS_CFLAGS)
LDFLAGS += $(TARGET_CPU_CFLAGS)
LDFLAGS += -g

OBJCOPY = $(RTEMS_TOOLS)/$(RTEMS_ARCH)-rtems$(RTEMS_VERSION)-objcopy

#TARGET_OS_SRC += $(TARGET_BSP_NETWORK_CONFIG_C)
TARGET_OS_SRC  += rtems_config.c
#TARGET_OS_SRC += config/rtems/rtems_filesystem.c
TARGET_OS_OBJ  = $(TARGET_OS_SRC:%.c=%.o)

#config/rtems/rtems_config.o: config/rtems/rtems_config.c \
#	config/rtems/rtems_filesystem.h config/rtems/rtems_network.h
#
#config/rtems/rtems_network.h: $(TARGET_BSP_NETWORK_CONFIG_H)
#	cp $(TARGET_BSP_NETWORK_CONFIG_H) config/rtems/rtems_network.h
#
#TARGET_CLEAN_ADDITIONS += rtems/rtems_network.h
#
#config/rtems/rtems_filesystem.c config/rtems/rtems_filesystem.h:
#	rm -rf rtems_rootfs
#	mkdir rtems_rootfs
#	cp config/rtems/README_RTEMS.md rtems_rootfs # XXX TMP
#	cd rtems_rootfs ; tar cf ../rtemsfs.tar *
#	$(RTEMS_TOOLS)/rtems-bin2c -H rtemsfs.tar config/rtems/rtems_filesystem.h
#	$(RTEMS_TOOLS)/rtems-bin2c -C rtemsfs.tar config/rtems/rtems_filesystem.c
#
#TARGET_CLEAN_ADDITIONS += config/rtems/rtems_filesystem.c
#TARGET_CLEAN_ADDITIONS += config/rtems/rtems_filesystem.h
#TARGET_CLEAN_ADDITIONS += rtemsfs.tar
#TARGET_CLEAN_ADDITIONS += rtems_rootfs
