#ifndef GETMODULESIZES_H
#define GETMODULESIZES_H

#include "patternloop.h"

#include <vector>
//#include <iostream>


class getModuleSizes : public patternloop {
 public:
  getModuleSizes(string ttree_file = "inputs/treeBank.root", int nevent = -1, int ievent = 0);
  virtual ~getModuleSizes(){};

  vector < vector < vector < int > > > getLayerLadderModule();
  vector < vector < vector < int > > > getLayerLadderModuleOld();
  int getStripsOn0();

  void doLoops();

 private:

  vector < int > ladders_per_layer;
  vector < vector < int > > modules_per_ladder_per_layer;

  int stripsOn0;

  void doLadderLoop();
  void doModuleLoop();
  void doStripsOn0Loop();
};

#endif
