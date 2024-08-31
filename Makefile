all: build
build:
	gcc -g *.c -o pong -Wall -lSDL2 -lSDL2_ttf
clean:
	rm pong