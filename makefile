
CC = clang
DEBUG = lldb
MKDIR = mkdir -p

ARGS =

TEST = NONE

FLAGS += -O2 -flto
# FLAGS += -g -fsanitize=address,undefined

LDFLAGS += ${FLAGS}
LDFLAGS += -framework Cocoa -framework IOKit

CFLAGS += -I raylib/include
# CFLAGS += -Wall -Wextra -Weverything -std=c11
CFLAGS += -std=c11 -pedantic -Wall -Wextra
# CFLAGS += -Wno-float-equal -Weverything -Wno-undef -Wno-declaration-after-statement -Wno-padded -Wno-poison-system-directories
CFLAGS += ${FLAGS}

SRCS :=
SRCS += src/camera.c
SRCS += src/color.c
SRCS += src/coord.c
SRCS += src/main.c
SRCS += src/render.c
SRCS += src/tris.c
SRCS += src/vector2.c
SRCS += src/worldgen.c

SRCS += src/pen/pen.c

SRCS += src/serial/serial.c

SRCS += src/float.c
SRCS += src/forth/forth.c
SRCS += src/forth/lib/core.c
SRCS += src/forth/lib/color.c

run: bins .dummy
	./out/bin/trieste ${ARGS}

build: bins

clean: .dummy
	rm -rf out

debug: bins
	${DEBUG} ./out/bin/trieste -- ${ARGS}

bins: out/bin/trieste

out/bin/trieste: ${SRCS:%.c=out/lib/%.o}
	@ ${MKDIR} ${dir ${@}}
	${CC} ${^} -o ${@} raylib/lib/libraylib.a ${LDFLAGS}

out/lib/%.o: %.c
	@ ${MKDIR} ${dir ${@}}
	${CC} -c ${^} -o ${@} ${CFLAGS}

.dummy:
