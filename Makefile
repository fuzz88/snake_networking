MODULES=main.c drawing.c snake.c game.c keyboard.c networking.c
LIBRARIES=-Iraylib-5.5_linux_amd64/include -Lraylib-5.5_linux_amd64/lib -l:libraylib.a -lm

build: main.c
	cc -ggdb -o main $(MODULES) $(LIBRARIES)

.PHONY: all