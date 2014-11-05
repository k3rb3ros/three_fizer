INCLUDES=-Iinclude
CFLAGS= $(STDFLAG) $(INCLUDES) $(DBGFLAGS) $(DEFINITIONS)
DEPS=$(wildcard include/*.h)
LDFLAGS=
OBJ=$(patsubst %.c,obj/%.o,$(SRC))
SRC=$(wildcard *.c)

threefizerObjects: $(OBJ)

$(OBJ): | obj

obj:
	@mkdir -p $@

obj/%.o:%.c $(SRC) $(DEPS)
	@echo $(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
	@$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	rm obj/*.o
