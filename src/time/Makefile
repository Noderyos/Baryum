CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude -I/usr/include/freetype2 -I/usr/include/libpng16
LDFLAGS=-lX11 -lXft
NAME_PLUGIN=time

all: $(NAME_PLUGIN).c concat.c
	$(CC) $(CFLAGS) -o $(NAME_PLUGIN) $? $(LDFLAGS)

clean:
	rm $(NAME_PLUGIN)
