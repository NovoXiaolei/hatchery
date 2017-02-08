CC=gcc
CFLAG=-c -Wall

all: select-test

select-test: select-test.o 
	@$(CC)  select-test.o -o select-test

select-test.o: select-test.c
	@$(CC) -c select-test.c 

clean:
	rm *.o select-test

