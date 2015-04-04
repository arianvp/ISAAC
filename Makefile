CFLAGS= -g -O3 -ansi -fPIC -pedantic -Wall -Wextra -Wno-unused-parameter -c

.PHONY: all clean

all: libisaac.a

libisaac.a: rand.o
	$(AR) rcs libisaac.a $^


rand.o: rand.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	$(RM) libisaac.a rand.o





