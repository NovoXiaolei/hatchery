CC=gcc 
CFLAG= -g -Wall
VPATH = ../src:../src/utils:../src/common

all: select-test log_test

select-test: select-test.o 
	@$(CC)  select-test.o -o select-test

select-test.o: select-test.c
	@$(CC) -c select-test.c 

log_test: log_test.c log.c log.h
	$(CC) $(CFLAG) $^ -o $@  -I../src/utils


clean:
	rm *.o select-test log_test

