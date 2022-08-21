PROJECT_NAME=jeep
BUILDOUT=bin
CC=gcc -w -std=c99 -Wall -Werror -Wextra 
LIBS=-lncurses -lm 
#EXPORTS=export LDFLAGS=-L/usr/lib/x86_64-linux-gnu/ # export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/

SOURCES = src/jeep.c

BUILDCMD_DIR = src/builtin
BUILDCMD_IN  = $(BUILDCMD_DIR)/sniff.c 
BUILDCMD_OUT = $(BUILDOUT)/sniff

INCLUDE_LIB = -I src/include/

all:
	rm -rf bin/ && rm -rf obj/
	mkdir -p bin/
	mkdir -p obj/

	# builtin cmds (bin/{sniff, ...})
	$(CC) $(INCLUDE_LIB) -pthread $(BUILDCMD_IN) -o $(BUILDCMD_OUT) $(LIBS)

	# kernel (bin/jeep)
	$(CC) $(INCLUDE_LIB) -pthread $(SOURCES) -o $(BUILDOUT)/$(PROJECT_NAME) $(LIBS)

	# obj 
	$(CC) $(INCLUDE_LIB) -pthread -c $(SOURCES) 
	mv *.o obj/

	# fix perms
	chmod +x bin/sniff
	chmod +x bin/jeep

jeep:
	rm -rf bin/ && rm -rf obj/
	mkdir bin/ obj/

	$(CC) -pthread $(SOURCES) -o $(BUILDOUT)/$(PROJECT_NAME) $(LIBS)
	chmod +x bin/jeep && bin/jeep --version &

clean:
	rm -f bin/$(PROJECT_NAME) *.o
	rm -rf obj/*
