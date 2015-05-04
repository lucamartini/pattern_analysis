ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)

CXX           = clang++
CXXFLAGS      = -fPIC -ansi -D_GNU_SOURCE -O2 -Wall -Wextra
#LDFLAGS       = -g3

ROOTLIBSFILTERED  = $(filter-out -lNew, $(ROOTGLIBS))

INCLUDEDIR       = ./
CXX	         += -I$(INCLUDEDIR)

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ../../../../../lib/

###########
# TARGETS #
###########

all: patterns patternsToStrips subpatterns

patterns: patterns.cc options plot
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -o patterns  $(ROOTLIBSFILTERED) patterns.cc options plot

patternsToStrips: patternsToStrips.cc options plot
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -o patternsToStrips  $(ROOTLIBSFILTERED) patternsToStrips.cc options plot

subpatterns: subpatterns.cc options plot
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -o subpatterns  $(ROOTLIBSFILTERED) subpatterns.cc options plot

options: options.cc
	$(CXX) -c $(CXXFLAGS) -o options options.cc

plot: plot.cc
	$(CXX) -c $(CXXFLAGS) $(ROOTCFLAGS) -o plot  plot.cc

clean:
	rm -rf patterns patternsToStrips subpatterns options plot
