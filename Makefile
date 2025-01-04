CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude
LDFLAGS=-lX11


all: src/main.c
	$(CC) $(CFLAGS) -o main $< $(LDFLAGS) -g -ggdb

clean:
	rm main
