CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude
LDFLAGS=
NAME_PLUGIN=time
OUT_DIR = ../../../plugins

all: $(NAME_PLUGIN).c
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$(NAME_PLUGIN) $? $(LDFLAGS)

clean:
	rm $(OUT_DIR)$(NAME_PLUGIN)
