CC = clang
CFLAGS = -Wall -I/opt/homebrew/Cellar/raylib/5.5/include
LIBS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo

game: game.c
	$(CC) $(CFLAGS) game.c -o game $(LIBS)

clean:
	rm -f game
