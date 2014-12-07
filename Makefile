# makefile for basic SIMLIB examples
# expects simlib.so in ..
# Linux

# installed library home:
SIMLIB_DIR=simlib/src

# dependencies:
SIMLIB_DEPEND=$(SIMLIB_DIR)/simlib.so $(SIMLIB_DIR)/simlib.h

# add flags:
CXXFLAGS += -I$(SIMLIB_DIR)

# Implicit rule to compile modules
% : %.cc
	@#$(CXX) $(CXXFLAGS) -static -o $@  $< $(SIMLIB_DIR)/simlib.a -lm
	$(CXX) $(CXXFLAGS) -o $@  $< $(SIMLIB_DIR)/simlib.so -lm

# Rules:
all: main

#
main: main.o $(SIMLIB_DEPEND)

#
run:
	./main

clean:
	rm -f main gmon.out

clean-all: clean clean-data
#
