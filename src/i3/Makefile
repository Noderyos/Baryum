CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude
LDFLAGS=
NAME_PLUGIN=i3plugin

all: $(NAME_PLUGIN).c
	$(CC) $(CFLAGS) -o ../../plugins/$(NAME_PLUGIN) $? $(LDFLAGS)

clean:
	rm $(NAME_PLUGIN)
