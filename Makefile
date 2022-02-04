CC = gcc
OPTS = -Wall -Werror

all: TUps

TUps: main.o options.o readProc.o
	$(CC) $(OPTS) -o TUps main.o options.o readProc.o

main.o: main.c
	$(CC) $(OPTS) -c main.c

options.o: options.c
	$(CC) $(OPTS) -c options.c
	
readProc.o: readProc.c
	$(CC) $(OPTS) -c readProc.c

clean:
	rm -f *.o TUps
	