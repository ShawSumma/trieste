
run: trieste
	./trieste

trieste: src/main.c
	clang src/main.c -o trieste -O2 raylib/lib/libraylib.a -I raylib/include -framework Cocoa -framework IOKit
