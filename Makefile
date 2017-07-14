CC=gcc -g
CFLAG=-c -Wall
MYSQL_INCLUDE=$(shell mysql_config --cflags)
MYSQL_LIBS=$(shell mysql_config --libs)
LUA_INCLUDE=/usr/local/include
LUA_LIB=/usr/local/lib/liblua.a
VPATH=src:src/client:src/common:src/server:src/utils

all: hatchery client

hatchery: hatchery.o log.o
	$(CC) hatchery.o log.o -o hatchery -I$(LUA_INCLUDE) -lm -llua $(LUA_LIB) -ldl

hatchery.o: hatchery.c log.h
	$(CC) $(CFLAG) $< -o hatchery.o -I$(LUA_INCLUDE) -lm -llua $(LUA_LIB) -ldl

log.o: log.c log.h
	$(CC)  $(CFLAG) $< -o log.o

client: client.o
	$(CC) client.o -o client  -ldl -lm

client.o: client.c common.h
	$(CC) $(CFLAG) $< -o client.o -ldl

.PHONY : clean
clean:
	rm *.o hatchery client
