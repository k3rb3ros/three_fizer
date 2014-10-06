INCLUDES=-Iinclude
CFLAGS= $(STDFLAG) $(INCLUDES) $(DBGFLAGS) $(DEFINITIONS)
DEPS=$(wildcard include/*.h)
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,obj/%.o,$(SRC))

threefizerObjects: $(OBJ)

$(OBJ): | obj

obj:
	@mkdir -p $@

obj/%.o:%.c $(SRC) $(DEPS)
	@echo $(CC) $(CFLAGS) -c $< -o $@
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm obj/*.o
