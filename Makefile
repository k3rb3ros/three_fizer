CC=clang
INCLUDES=-Isrc/include
CFLAGS=$(OPTFLAGS) $(INCLUDES) -std=c11 -Wall -Wextra -pedantic -g -DDEBUG
LDFLAGS=
SOURCES=src/*.c*
OBJECTS=$(SOURCES:.c=.o)
OPTFLAGS=$(-O0)
BINARY=threefizer

all: $(BINARY) $(OBJECTS)
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm $(BINARY) *.o
