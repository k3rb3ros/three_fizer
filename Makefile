CC=clang
DBGFLAGS=-ggdb -Wall -Wextra -pedantic
STDFLAG=-std=c11
DEFINITIONS=-DDEBUG
INCLUDES=-Isrc/include
CFLAGS= $(STDFLAG) $(INCLUDES) $(DBGFLAGS) $(DEFINITIONS)
LDFLAGS=
PRGRAM_OBJECTS=$(patsubst src/%.c, src/obj/%.o, $(wildcard src/*.c))
TEST_OBJECTS=$(patsubst src/*.c, src/test/obj/%.o, $(wildcard src/test/*.c)) 
OPTFLAGS=-O0
PROGRAM=threefizer
TEST=tests
export CC
export DBGFLAGS
export STDFLAG
export DEFINITIONS
export LDFLAGS
export OPTFLAGS

all: PROGRAM TEST

PROGRAM: threefizer
	$(CC) $(CFLAGS) $(PRGRAM_OBJECTS) -o $(PROGRAM)

TEST: test
	$(CC) $(CFLAGS) $(TEST_OBJECTS) -o $(TEST)
	
.PHONY: clean

clean:
	cd src; make clean
	cd src/test; make clean
	rm $(PROGRAM) $(TEST)

threefizer:
	cd src; make

test:
	cd src/test; make
