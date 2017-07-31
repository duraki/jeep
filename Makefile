PROJECT_NAME=jeep
CC=gcc

SOURCES = src/jeep.c

all:
	$(CC) -pthread $(SOURCES) -o obj/$(PROJECT_NAME)

clean:
	rm -f $(PROJECT_NAME) *.o
