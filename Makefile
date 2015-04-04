#CC=clang
DBGFLAGS=-ggdb -Wall -Wextra -pedantic
STDFLAG=-std=c11
#DEFINITIONS=-DDEBUG
#DEFINITIONS=-DDEBUG2
#DEFINITIONS=-DDEBUG3
INCLUDES=-Isrc/include
CFLAGS= $(STDFLAG) $(INCLUDES) $(DBGFLAGS) $(DEFINITIONS)
LDFLAGS=-lpthread -lgcrypt
PRGRAM_OBJECTS=$(patsubst src/%.c, src/obj/%.o, $(wildcard src/*.c))
TEST_OBJECTS=$(patsubst src/test/%.c, src/test/obj/%.o, $(wildcard src/test/*.c)) 
OPTFLAGS=-O2
PROGRAM=threefizer
TEST=tests
export CC
export DBGFLAGS
export STDFLAG
export DEFINITIONS
export OPTFLAGS
export PRGRAM_OBJECTS

.PHONY: clean

all: program tests

program: threefizer
	$(CC) $(CFLAGS) $(PRGRAM_OBJECTS) $(LDFLAGS) -o $(PROGRAM)

tests: threefizer test
	$(eval TF_OBJECTS=$(filter-out src/obj/threefizer.o, $(PRGRAM_OBJECTS)))
	$(CC) $(CFLAGS) $(TEST_OBJECTS) $(TF_OBJECTS) $(LDFLAGS) -o $(TEST)
	
clean:
	cd src; make clean
	cd src/test; make clean
	rm $(PROGRAM) $(TEST)

threefizer:
	cd src; make

test:
	cd src/test; make