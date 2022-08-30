#
# Makefile configuration to be shared and pull in the target
# dependent one
#

ifeq ($(CONFIG_INC),)
  CONFIG_INC=config/linux.inc
endif

ifeq (${TOPDIR},)
  TOPDIR=..
endif

include ${TOPDIR}/$(CONFIG_INC)

# Implicit linking rule when EXEEXT=exe
%.exe:
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

