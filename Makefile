CC=gcc
CFLAG=-c -Wall

all: hatchery

hatchery: main.o
	@$(CC) main.o -o hatchery -lpthread

main.o: main.c
	@$(CC) -c main.c -lpthread

clean:
	rm *.o hatchery
