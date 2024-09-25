all: build
build:
	gcc -g *.c -o pong -Wall -lSDL2 -lSDL2_ttf
clean:
	rm pong
win:
	i686-w64-mingw32-gcc main.c -o main.exe -Ii686-w64-mingw32/include/ -Li686-w64-mingw32/lib/lSDL2-Li686-w64-mingw32/lib/lSDL2_ttf