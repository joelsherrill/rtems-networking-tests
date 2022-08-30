TOPDIR=$(CURDIR)
include config.inc

SUBDIRS += devctl_socket
SUBDIRS += tcp_echo_client
SUBDIRS += tcp_echo_server
SUBDIRS += udp_echo_server
SUBDIRS += fcntl_socket
SUBDIRS += getaddrinfo01
SUBDIRS += getnameinfo01
SUBDIRS += ioctl_socket
SUBDIRS += mcast_server
SUBDIRS += mcast_client
SUBDIRS += select01
SUBDIRS += send_recv
SUBDIRS += sockopt01

ifeq ($(TCPIP_STACK),libbsd)
SUBDIRS += libbsd_cfg_space
endif

TESTS_TO_RUN += devctl_socket/devctl_socket$(EXEEXT)
MANUAL_TESTS += tcp_echo_client/tcp_echo_client$(EXEEXT)
MANUAL_TESTS += tcp_echo_server/tcp_echo_server$(EXEEXT)
MANUAL_TESTS += udp_echo_server/udp_echo_server$(EXEEXT)
TESTS_TO_RUN += fcntl_socket/fcntl_socket$(EXEEXT)
MANUAL_TESTS += getaddrinfo01/getaddrinfo01$(EXEEXT)
TESTS_TO_RUN += getnameinfo01/getnameinfo01$(EXEEXT)
TESTS_TO_RUN += ioctl_socket/ioctl_socket$(EXEEXT)
MANUAL_TESTS += mcast_listener/mcast_server$(EXEEXT)
MANUAL_TESTS += mcast_writer/mcast_client$(EXEEXT)
MANUAL_TESTS += select01/select01$(EXEEXT)
TESTS_TO_RUN += send_recv/send_recv$(EXEEXT)
TESTS_TO_RUN += sockopt01/sockopt01$(EXEEXT)

ifeq ($(TCPIP_STACK),libbsd)
TESTS_TO_RUN += libbsd_cfg_space
endif

TOPTARGETS := all clean
TOPTARGETS := all clean

SILENT = --no-print-directory 

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) $(SILENT) -C $@  TOPDIR=$(CURDIR) $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)

ifneq ($(TARGET_OS_RUNNER),)
  TARGET_OS_RUNNER_ARG= -r $(TARGET_OS_RUNNER)
endif

runtests:
	./run_tests $(TARGET_OS_RUNNER_ARG) $(TESTS_TO_RUN)

print_manual_tests:
ifeq ($(MANUAL_TESTS),)
	@echo "There are no manually run tests"
else
	@echo "Manual Tests:"
	@for t in $(MANUAL_TESTS) ; do \
	    echo $$t ; \
	done
endif
