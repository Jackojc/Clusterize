CC = g++

SRC = main.cpp
DLL = examples/sum.cpp
OUTDIR = build/

LDFLAGS = -ldl -O3


all:
	$(CC) $(SRC) $(LDFLAGS) -o $(OUTDIR)bin.out
	$(CC) -shared -o $(OUTDIR)sum.so -fPIC $(DLL)


.PHONY: clean
clean:
	rm -rf build
	mkdir build