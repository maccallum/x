all: x.test lib/libx.dylib

x.test: x.test.o x.capi.o x.capi-callbacks.o
	clang++ x.test.o x.capi.o x.capi-callbacks.o -o x.test

lib/libx.dylib: x.capi.o
	clang -shared -undefined dynamic_lookup -o lib/libx.dylib x.capi.o x.capi-callbacks.o

x.test.o: x.test.c
	clang -gfull -ggdb2 -c x.test.c -I./include -o x.test.o

x.capi.o: x.capi.cpp
	clang++ -gfull -ggdb2 -std=c++17 -c x.capi.cpp -I./include -I./pcg-cpp/include -o x.capi.o

x.capi-callbacks.o: x.capi-callbacks.c
	clang -gfull -ggdb2 -c x.capi-callbacks.c -I./include -o x.capi-callbacks.o

.PHONY: clean
clean:
	rm -rf *.o;
	rm -rf x.test
	rm -rf lib/libx.dylib
