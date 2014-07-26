CC=clang
PP=clang++
INCLUDES=-Isrc/include
CFLAGS=$(OPTFLAGS) $(INCLUDES) -Wall -Wextra -pedantic -g -DDEBUG
LDFLAGS=
SOURCES=src/*.c*
OBJECTS=$(SOURCES:.c=.o)
OPTFLAGS=$(-O0)
BINARY=threefizer

all: $(BINARY) $(OBJECTS)
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(PP) $(CFLAGS) -c $< -o $@

$(BINARY): $(OBJECTS)
	$(PP) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm $(BINARY) *.o
