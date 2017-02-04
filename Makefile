all: hetchery

hetchery: main.o
	@gcc main.o -o hetchery

main.o: main.c
	@gcc -c main.c

clean:
	rm *.o hetchery
