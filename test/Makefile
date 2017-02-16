CC=gcc -g
CFLAG=-c -Wall
MYSQL_INCLUDE=$(shell mysql_config --cflags)
MYSQL_LIBS=$(shell mysql_config --libs)

all: select-test mysql_test

select-test: select-test.o 
	@$(CC)  select-test.o -o select-test

select-test.o: select-test.c
	@$(CC) -c select-test.c 

mysql_test: mysql_test.c
	@$(CC) $(MYSQL_INCLUDE) mysql_test.c -o  mysql_test $(MYSQL_LIBS)


clean:
	rm *.o select-test mysql_test

