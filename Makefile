CC=gcc
CFLAG=-c -Wall

all: hatchery

hatchery: main.o
	@$(CC) main.o -o hatchery

main.o: main.c
	@$(CC) -c main.c

clean:
	rm *.o hatchery
