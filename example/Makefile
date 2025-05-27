
CC = clang

SRC = main.c configs.c ../src/cli_loop.c ../src/base_configs.c
INC = ../include
DST = build

all: clean
	mkdir -p $(DST)
	clang $(SRC) -I$(INC) -o build/a.out

debug: clean
	mkdir -p $(DST)
	clang -g $(SRC) -I$(INC) -o build/a.out

clean:
	rm -rf $(DST)