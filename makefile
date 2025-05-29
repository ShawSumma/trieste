
CC = gcc
DEBUG = lldb

TEST = NONE

CFLAGS += -O3 -flto
# CFLAGS += -fsanitize=address,undefined
CFLAGS += -I raylib/include
CFLAGS += -Wall -Wextra -Wconversion -std=c11 -pedantic

SRCS :=
SRCS += src/camera.c
SRCS += src/color.c
SRCS += src/coord.c
SRCS += src/float.c
SRCS += src/forth/forth.c
SRCS += src/forth/library.c
SRCS += src/main.c
SRCS += src/render.c
SRCS += src/tris.c
SRCS += src/vector2.c
SRCS += src/worldgen.c

run: build
	./trieste

debug: build
	${DEBUG} ./trieste

build: trieste

trieste: src/main.c
	${CC} ${SRCS} -o trieste raylib/lib/libraylib.a -framework Cocoa -framework IOKit -DTEST_${TEST} ${CFLAGS}
