CC=gcc
CFLAGS=-Wall
LINKFLAGS=-Wall
DIR=src

all: bin/cade

clean:
	rm -rf bin obj

bin:
	mkdir -p bin

bin/cade: bin obj/agent.o obj/group.o obj/main.o
	$(CC) -g -o bin/cade obj/* $(LINKFLAGS)

obj:
	mkdir -p obj

obj/agent.o: obj agent.c agent.h
	$(CC) -g $(CFLAGS) -c agent.c -o obj/agent.o

obj/main.o: obj main.c
	$(CC) -g $(CFLAGS) -c main.c -o obj/main.o

obj/group.o: obj wrap.cc wrap.hh qosfs.hh
	$(CC) -g $(CFLAGS) -c group.c -o obj/group.o
