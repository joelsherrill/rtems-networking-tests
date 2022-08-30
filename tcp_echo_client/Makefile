include ../config.inc

PROGRAM=tcp_echo_client

MANUAL_TESTS += $(PROGRAM)$(EXEEXT)

all: $(PROGRAM)$(EXEEXT)

$(PROGRAM)$(EXEEXT): $(PROGRAM).o $(TARGET_OS_OBJ)

$(PROGRAM).o: $(PROGRAM).c

clean:
	rm -f $(PROGRAM) $(PROGRAM).exe *.o
