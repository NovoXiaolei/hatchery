CC=gcc
CFLAG= -g -Wall -lm -ldl $(INCLUDE)
MYSQL_INCLUDE=$(shell mysql_config --cflags)
MYSQL_LIBS=$(shell mysql_config --libs)
INCLUDE=-Isrc/common -Isrc/utils
LUA_INCLUDE=lua
LUA_LIB=lua/liblua.a
VPATH=src:src/client:src/common:src/server:src/utils

all: hatchery client

hatchery: hatchery.c log.c log.c log.h common.h
	$(CC) $(CFLAG) $^ -o $@  -I$(LUA_INCLUDE) $(LUA_LIB)

client: client.o
	$(CC) client.o -o client

client.o: client.c common.h
	$(CC) -c $<

.PHONY : clean
clean:
	rm *.o hatchery client
