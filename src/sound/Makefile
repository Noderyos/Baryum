CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude
LDFLAGS=-lpulse


all: sound.c pulse_interface.c
	$(CC) $(CFLAGS) -o sound $? $(LDFLAGS)

clean:
	rm sound
