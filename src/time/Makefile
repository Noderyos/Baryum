CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude
LDFLAGS=
NAME_PLUGIN=time

all: $(NAME_PLUGIN).c concat.c
	$(CC) $(CFLAGS) -o $(NAME_PLUGIN) $? $(LDFLAGS)

clean:
	rm $(NAME_PLUGIN)
