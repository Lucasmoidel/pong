all: build
build:
	gcc -g *.c -o pong -Wall -lSDL2 -lSDL2_ttf
clean:
	rm pong
win:
	x86_64-w64-mingw32-gcc *.c -o pong.exe -Wall -g -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
