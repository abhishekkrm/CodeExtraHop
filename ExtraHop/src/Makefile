CC = gcc
CCOPTS = -c -g -Wall
LINKOPTS = -ggdb

all: ExtraHop

ExtraHop: ExtraHop.o hash.o
	$(CC) $(LINKOPTS) -o  $@ $^

ExtraHop.o: ExtraHop.c ExtraHop.h hash.h
	$(CC) $(CCOPTS) -o $@ ExtraHop.c

hash.o: hash.c hash.h
	$(CC) $(CCOPTS) -o $@ hash.c

clean:
	rm -rf *.o *~ ExtraHop
