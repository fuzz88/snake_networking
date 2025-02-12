.PHONY: all

build: main.c
	cc -ggdb -o main main.c drawing.c snake.c game.c keyboard.c -Iraylib-5.5_linux_amd64/include -Lraylib-5.5_linux_amd64/lib -l:libraylib.a -lm
