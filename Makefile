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

all: patternsToStrips subpatterns drawgeometry

patternsToStrips: patternsToStrips.cc options plot
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -o patternsToStrips  $(ROOTLIBSFILTERED) patternsToStrips.cc options plot

subpatterns: subpatterns.cc options plot getModuleSizes makesubbanks
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -o subpatterns  $(ROOTLIBSFILTERED) subpatterns.cc options plot getModuleSizes patternloop makesubbanks

subpatterns_l5: subpatterns_l5.cc options plot getModuleSizes
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -o subpatterns_l5  $(ROOTLIBSFILTERED) subpatterns_l5.cc options plot getModuleSizes

drawgeometry: drawgeometry.cc options plot
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -o drawgeometry  $(ROOTLIBSFILTERED) drawgeometry.cc options plot

options: options.cc
	$(CXX) -c $(CXXFLAGS) -o options options.cc

plot: plot.cc
	$(CXX) -c $(CXXFLAGS) $(ROOTCFLAGS) -o plot  plot.cc

makesubbanks:: makesubbanks.cc patternloop
	$(CXX) -c $(CXXFLAGS) $(ROOTCFLAGS) -o makesubbanks  makesubbanks.cc

getModuleSizes: getModuleSizes.cc patternloop
	$(CXX) -c $(CXXFLAGS) $(ROOTCFLAGS) -o getModuleSizes  getModuleSizes.cc

patternloop: patternloop.cc
	$(CXX) -c $(CXXFLAGS) $(ROOTCFLAGS) -o patternloop  patternloop.cc


clean:
	rm -rf patternsToStrips subpatterns subpatterns_l5 drawgeometry options plot getModuleSizes patternloop makesubbanks
