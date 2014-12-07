
SIMLIB_DIR=simlib/src

CXX=g++ -m64
CXXFLAGS  = -Wall -std=c++98 -O2
CXXFLAGS += -I$(SIMLIB_DIR)

SIMLIB_DEPEND=$(SIMLIB_DIR)/simlib.so $(SIMLIB_DIR)/simlib.h

% : %.cc
    @#$(CXX) $(CXXFLAGS) -static -o $@  $< $(SIMLIB_DIR)/simlib.a -lm
    $(CXX) $(CXXFLAGS) -o $@  $< $(SIMLIB_DIR)/simlib.so -lm

# Rules:
all: dol
    

dol: main lod $(SIMLIB_DEPEND)
    

run:
    ./main

clean:
    rm -f main gmon.out

clean-all: clean clean-data
#
