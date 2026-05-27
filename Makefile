CC = gcc
CFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	mkdir -p build
	$(CC) src/*.c -o build/jogo $(CFLAGS)

run: all
	./build/jogo

clean:
	rm -f build/jogo
