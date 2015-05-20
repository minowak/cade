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

obj/agent.o: obj
	$(CC) -g $(CFLAGS) -c $(DIR)/agent.c -o obj/agent.o

obj/main.o: obj
	$(CC) -g $(CFLAGS) -c $(DIR)/main.c -o obj/main.o

obj/group.o: obj
	$(CC) -g $(CFLAGS) -c $(DIR)/group.c -o obj/group.o
