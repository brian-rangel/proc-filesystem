CC = gcc
OPTS = -Wall -Werror

all: TUps

TUps: main.o readProc.o
	$(CC) $(OPTS) -o TUps main.o readProc.o

main.o: main.c
	$(CC) $(OPTS) -c main.c
	
readProc.o: readProc.c
	$(CC) $(OPTS) -c readProc.c

clean:
	rm -f *.o TUps