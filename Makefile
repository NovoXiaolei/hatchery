CC=gcc -g
CFLAG=-c -Wall
MYSQL_INCLUDE=$(shell mysql_config --cflags)
MYSQL_LIBS=$(shell mysql_config --libs)
VPATH=./src:./src/server/

all: hatchery client

hatchery: hatchery.o
	@$(CC) hatchery.o -o hatchery

hatchery.o: hatchery.c
	@$(CC) -c hatchery.c

main.o: main.c
	@$(CC) -c main.c -lpthread

client: client.o
	@$(CC) client.o -o client -lpthread

client.o: client.c
	@$(CC) -c client.c -lpthread

.PHONY : clean
clean:
	rm *.o hatchery client
