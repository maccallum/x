c: x.test

x.test: x.test.o x.capi.o
	clang++ x.test.o x.capi.o -o x.test

x.test.o: x.test.c
	clang -c x.test.c -I./include -o x.test.o

x.capi.o: x.capi.cpp
	clang++ -std=c++17 -c x.capi.cpp -I./include -I./pcg-cpp/include -o x.capi.o

# x.test: x.test.o x.wrapper.random_device.o x.wrapper.seed_seq_from.o x.wrapper.seed_seq_from_delegate.o
# 	clang++ x.test.o x.wrapper.random_device.o x.wrapper.seed_seq_from.o x.wrapper.seed_seq_from_delegate.o -o x.test

# x.test.o: x.test.c
# 	clang -c x.test.c -I./include -o x.test.o

# x.wrapper.random_device.o: x.wrapper.random_device.cpp x.wrapper.random_device.h
# 	clang++ -std=c++17 -c x.wrapper.random_device.cpp -I./include -o x.wrapper.random_device.o

# x.wrapper.seed_seq_from.o: x.wrapper.seed_seq_from.cpp x.wrapper.seed_seq_from.h
# 	clang++ -std=c++17 -c x.wrapper.seed_seq_from.cpp -I./include -I./pcg-cpp/include -o x.wrapper.seed_seq_from.o

# x.wrapper.seed_seq_from_delegate.o: x.wrapper.seed_seq_from_delegate.cpp x.wrapper.seed_seq_from_delegate.h
# 	clang++ -std=c++17 -c x.wrapper.seed_seq_from_delegate.cpp -I./include -I./pcg-cpp/include -o x.wrapper.seed_seq_from_delegate.o

# .PHONY: x.wrapper.random_device.h x.wrapper.seed_seq_from.h x.wrapper.seed_seq_from_delegate.h

.PHONY: clean
clean:
	rm -rf *.o;
	rm -rf x.test

