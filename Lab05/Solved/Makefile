build: tetris

tetris: tetris.c pieces.h
	gcc -o tetris tetris.c -Wall -Wextra -lallegro -lallegro_primitives -lallegro_image

.PHONY: clean

clean:
	rm tetris
