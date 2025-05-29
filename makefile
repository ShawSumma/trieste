
CC = gcc
DEBUG = lldb

TEST = NONE

CFLAGS += -O3 -flto
CFLAGS += -fsanitize=address,undefined
CFLAGS += -I raylib/include

SRCS = src/camera.c src/main.c src/tris.c

run: build
	./trieste

debug: build
	${DEBUG} ./trieste

build: trieste

trieste: src/main.c
	${CC} ${SRCS} -o trieste raylib/lib/libraylib.a -framework Cocoa -framework IOKit -DTEST_${TEST} ${CFLAGS}
