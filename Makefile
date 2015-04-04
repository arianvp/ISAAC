CFLAGS= -g -O3 -ansi -pedantic -Wall -Wextra -Wno-unused-parameter

ifneq ($(OS), Windows_NT)
	CFLAGS += -fPIC

	ifeq ($(shell uname), Darwin)
		LDFLAGS += -dynamiclib -undefined dynamic_lookup
	endif
endif

.PHONY: all clean

all: isaac.so

isaac.so: rand.c
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ rand.c

clean:
	$(RM) isaac.so





