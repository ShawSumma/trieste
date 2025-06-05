
CC = clang
DEBUG = lldb

ARGS =

TEST = NONE

# CFLAGS += -O2 -flto
CFLAGS += -g -fsanitize=address,undefined
CFLAGS += -I raylib/include
CFLAGS += -Wall -Wextra -Wconversion -std=c11 -pedantic
CFLAGS += -ferror-limit=1

SRCS :=
SRCS += src/camera.c
SRCS += src/color.c
SRCS += src/coord.c
SRCS += src/main.c
SRCS += src/render.c
SRCS += src/tris.c
SRCS += src/vector2.c
SRCS += src/worldgen.c

SRCS += src/serial/serial.c

SRCS += src/float.c
SRCS += src/forth/forth.c
SRCS += src/forth/lib/core.c
SRCS += src/forth/lib/color.c

run: build
	./bin/trieste ${ARGS}

debug: build
	${DEBUG} ./bin/trieste -- ${ARGS}

build: bin/trieste

bin/trieste: ${SRCS}
	${CC} ${SRCS} -o bin/trieste raylib/lib/libraylib.a -framework Cocoa -framework IOKit -DTEST_${TEST} ${CFLAGS}
