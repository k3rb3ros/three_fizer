CC=clang
DBGFLAGS=-ggdb -Wall -Wextra -pedantic
STDFLAG=-std=c11
DEFINITIONS=-DDEBUG
INCLUDES=-Isrc/include
CFLAGS= $(STDFLAG) $(INCLUDES) $(DBGFLAGS) $(DEFINITIONS)
LDFLAGS=
PRGRAM_OBJECTS=$(patsubst src/%.c, src/obj/%.o, $(wildcard src/*.c))
TEST_OBJECTS=$(patsubst src/test/%.c, src/test/obj/%.o, $(wildcard src/test/*.c)) 
OPTFLAGS=-O0
PROGRAM=threefizer
TEST=tests
export CC
export DBGFLAGS
export STDFLAG
export DEFINITIONS
export LDFLAGS
export OPTFLAGS
export PRGRAM_OBJECTS

.PHONY: clean

all: program tests

program: threefizer
	$(CC) $(CFLAGS) $(PRGRAM_OBJECTS) -o $(PROGRAM)

tests: threefizer test
	$(eval TF_OBJECTS=$(filter-out src/obj/threefizer.o, $(PRGRAM_OBJECTS)))
	$(CC) $(CFLAGS) $(TEST_OBJECTS) $(TF_OBJECTS) -o $(TEST)
	
clean:
	cd src; make clean
	cd src/test; make clean
	rm $(PROGRAM) $(TEST)

threefizer:
	cd src; make

test:
	cd src/test; make
