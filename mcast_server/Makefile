include ../config.inc

PROGRAM=mcast_server

MANUAL_TESTS += $(PROGRAM)$(EXEEXT)

all: $(PROGRAM)$(EXEEXT)

$(PROGRAM)$(EXEEXT): $(PROGRAM).o $(TARGET_OS_OBJ)

$(PROGRAM).o: $(PROGRAM).c

clean:
	rm -f $(PROGRAM) $(PROGRAM).exe *.o
