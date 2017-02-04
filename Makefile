all: hatchery

hatchery: main.o
	@gcc main.o -o hatchery

main.o: main.c
	@gcc -c main.c

clean:
	rm *.o hatchery
