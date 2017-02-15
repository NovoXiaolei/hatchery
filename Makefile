CC=gcc -g
CFLAG=-c -Wall
MYSQL_INCLUDE=$(shell mysql_config --cflags)
MYSQL_LIBS=$(shell mysql_config --libs)

all: hatchery client

hatchery: main.o
	@$(CC) main.o -o hatchery -lpthread

main.o: main.c
	@$(CC) -c main.c -lpthread

client: client.o
	@$(CC) client.o -o client -lpthread

client.o: client.c
	@$(CC) -c client.c -lpthread

clean:
	rm *.o hatchery client
