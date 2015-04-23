ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)

CXX           = clang++
CXXFLAGS      = -fPIC -ansi -D_GNU_SOURCE -O2 -Wall -Wextra
#LDFLAGS       = -g3

CXXFLAGS      += $(ROOTCFLAGS)
NGLIBS         = $(ROOTGLIBS)
GLIBS          = $(filter-out -lNew, $(NGLIBS))

INCLUDEDIR       = ./
CXX	         += -I$(INCLUDEDIR)

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ../../../../../lib/

###########
# TARGETS #
###########

patterns: patterns.cc
	$(CXX) $(CXXFLAGS) -o patterns $(GLIBS)  $(LDFLAGS)  $<

makeTrees: makeTrees.cc
	$(CXX) $(CXXFLAGS) -o makeTrees $(GLIBS)  $(LDFLAGS)  $<

clean:
	rm -rf patterns
