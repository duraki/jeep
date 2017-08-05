PROJECT_NAME=jeep
CC=gcc -w -std=c99

SOURCES = src/jeep.c

all:
	$(CC) -pthread -c $(SOURCES) 
	$(CC) -pthread $(SOURCES) -o bin/$(PROJECT_NAME) 
	mv *.o obj/

clean:
	rm -f bin/$(PROJECT_NAME) *.o
	rm -rf obj/*
