main: main.cc lod.cc
	g++ -g -O2 -o dol main.cc lod.cc -lsimlib -lm

all: main

run:
	./main

clean:
	rm -f *.o dol gmon.out

clean-all: clean 
