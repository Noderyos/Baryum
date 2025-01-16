CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude
LDFLAGS=
NAME_PLUGIN=active

all: $(NAME_PLUGIN).c
	$(CC) $(CFLAGS) -o ../../plugins/$(NAME_PLUGIN) $? $(LDFLAGS)

clean:
	rm $(NAME_PLUGIN)
