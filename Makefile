PROJECT_NAME=jeep
BUILDOUT=bin
CC=gcc -w -std=c99

SOURCES = src/jeep.c

BUILDCMD_DIR = src/builtin
BUILDCMD_IN  = $(BUILDCMD_DIR)/sniff.c 
BUILDCMD_OUT = $(BUILDOUT)/sniff

all:
	# bin
	# builtin cmds
	$(CC) -pthread -lncurses $(BUILDCMD_IN) -o $(BUILDCMD_OUT)

  # kernel
	$(CC) -pthread -lncurses $(SOURCES) -o $(BUILDOUT)/$(PROJECT_NAME) 

  # obj 
	$(CC) -pthread -c $(SOURCES) 
	mv *.o obj/

clean:
	rm -f bin/$(PROJECT_NAME) *.o
	rm -rf obj/*
