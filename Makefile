CC=gcc
CFLAG= -g -Wall -lm -ldl -I$(LUA_INCLUDE) 
MYSQL_INCLUDE=$(shell mysql_config --cflags)
MYSQL_LIBS=$(shell mysql_config --libs)
LUA_INCLUDE=lua
LUA_LIB=lua/liblua.a
VPATH=src:src/client:src/common:src/server:src/utils

all: hatchery client

hatchery: hatchery.o log.o
	$(CC) $(CFLAG) $^ -o $@ $(LUA_LIB)

hatchery.o: hatchery.c common.h log.h
	$(CC) $(CFLAG) -c $< 

log.o: log.c log.h
	$(CC) -c  $<

client: client.o
	$(CC) client.o -o client

client.o: client.c common.h
	$(CC) -c $<

.PHONY : clean
clean:
	rm *.o hatchery client
