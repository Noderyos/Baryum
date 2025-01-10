CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude -I/usr/include/freetype2 -I/usr/include/libpng16
LDFLAGS=-lX11 -lXft -lpulse


all: sound.c pulse_interface.c
	$(CC) $(CFLAGS) -o sound $? $(LDFLAGS)

clean:
	rm sound
