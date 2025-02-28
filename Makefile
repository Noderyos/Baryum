CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude -I/usr/include/freetype2 -I/usr/include/libpng16
LDFLAGS=-lX11 -lXft -lXcomposite


all: src/main.c src/draw.c src/toast.c src/plugins_manager.c src/toml/toml_lexer.c src/toml/toml.c 
	$(CC) $(CFLAGS) -o main $? $(LDFLAGS)

clean:
	rm main
