all: x.test libx.dylib

x.test: x.test.o x.capi.o
	clang++ x.test.o x.capi.o -o x.test

libx.dylib: x.capi.o
	clang -shared -undefined dynamic_lookup -o libx.dylib x.capi.o

x.test.o: x.test.c
	clang -c x.test.c -I./include -o x.test.o

x.capi.o: x.capi.cpp
	clang++ -std=c++17 -c x.capi.cpp -I./include -I./pcg-cpp/include -o x.capi.o

.PHONY: clean
clean:
	rm -rf *.o;
	rm -rf x.test
	rm -rf libx.dylib

