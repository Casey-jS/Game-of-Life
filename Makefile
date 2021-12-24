CC=gcc

all: conway.c board.c board.h
	$(CC) -o conway.out conway.c board.c

clean: 
	rm conway.out