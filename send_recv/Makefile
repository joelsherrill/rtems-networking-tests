include ../config.inc

PROGRAM=send_recv

TESTS_TO_RUN += $(PROGRAM)$(EXEEXT)

all: $(PROGRAM)$(EXEEXT)

$(PROGRAM)$(EXEEXT): $(PROGRAM).o $(TARGET_OS_OBJ)

$(PROGRAM).o: $(PROGRAM).c

clean:
	rm -f $(PROGRAM) $(PROGRAM).exe *.o
