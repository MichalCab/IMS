main: main.cc lod.cc
	g++ -g -O2 -o dol main.cc lod.cc -lsimlib -lm

all: main

run:
	./dol

clean:
	rm -f *.o dol gmon.out

clean-all: clean 

pack:
	02_xilavs01_xcabmi00.zip Dokumentacia.pdf Makefile lod.h main.cc lod.cc
