CC=gcc
CFLAGS=
LINKFLAGS=-lpthread
DIR=src

all: bin/agent bin/group

clean:
	rm -rf bin obj

bin:
	mkdir -p bin

bin/agent: bin obj/agent.o
	$(CC) -g -o bin/agent obj/agent.o $(LINKFLAGS)

bin/group: bin obj/group.o
	$(CC) -g -o bin/group obj/group.o $(LINKFLAGS)

obj:
	mkdir -p obj

obj/agent.o: obj
	$(CC) -g $(CFLAGS) -c $(DIR)/agent.c -o obj/agent.o

obj/group.o: obj
	$(CC) -g $(CFLAGS) -c $(DIR)/group.c -o obj/group.o
