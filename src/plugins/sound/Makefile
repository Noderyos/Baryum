CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude
LDFLAGS=-lpulse
NAME_PLUGIN=sound
OUT_DIR = ../../../plugins

all: $(NAME_PLUGIN).c pulse_interface.c draw_type.c
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$(NAME_PLUGIN) $? $(LDFLAGS)

clean:
	rm $(OUT_DIR) $(NAME_PLUGIN)
