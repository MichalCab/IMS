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
all: dol

#
dol:   dol.cc $(SIMLIB_DEPEND)

#
run:
	./dol

plot: run
	@./0-plot-all-ps.sh

clean:
	rm -f $(ALL_MODELS) gmon.out

clean-data:
	rm -f *.dat *.out *.png *.ps reference-outputs/*.log

clean-all: clean clean-data

REFOUT=reference-outputs/*.dat reference-outputs/*.out reference-outputs/*.sh

pack:
	tar czf examples.tar.gz *.cc *.plt *.txt *.sh Makefile.* $(REFOUT)

#
