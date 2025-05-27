
CC = gcc
DEBUG = lldb

TEST = NONE

CFLAGS += -O3
CFLAGS += -fsanitize=address,undefined
CFLAGS += -I raylib/include

run: build
	./trieste

debug: build
	${DEBUG} ./trieste

build: trieste

trieste: src/main.c
	${CC} src/main.c -o trieste raylib/lib/libraylib.a -framework Cocoa -framework IOKit -DTEST_${TEST} ${CFLAGS}
