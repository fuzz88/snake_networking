.PHONY: all

build: main.c
	cc -o main main.c -Iraylib-5.5_linux_amd64/include -Lraylib-5.5_linux_amd64/lib -l:libraylib.a -lm
