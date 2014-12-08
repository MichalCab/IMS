SIMLIB_DIR=simlib/src

CXX=g++ -m64
CXXFLAGS  = -Wall -std=c++98 -O2
CXXFLAGS += -I$(SIMLIB_DIR)

SIMLIB_DEPEND=$(SIMLIB_DIR)/simlib.so $(SIMLIB_DIR)/simlib.h

main: main.cc lod.cc
	$(CXX) $(CXXFLAGS) main.cc lod.cc -o dol $(SIMLIB_DIR)/simlib.so -lm

lod.o: lod.cc
	$(CXX) $(CXXFLAGS) -c lod.cc

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c main.cc



# Rules:
all: main $(SIMLIB_DEPEND)  

run:
	./main

clean:
	rm -f *.o main gmon.out

clean-all: clean clean-data
#
