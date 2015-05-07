#ifndef GETMODULESIZES_H
#define GETMODULESIZES_H

#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

const unsigned int LAYERS = 6;

class getModuleSizes {
 public:
  getModuleSizes(string ttree_file = "inputs/treeBank.root");
  vector < vector < vector < int > > > getLayerLadderModule();
  vector < vector < vector < int > > > getLayerLadderModuleOld();

 private:
  TFile * input_f;
  TTree * tree;

  vector < int > ladders_per_layer;
  vector < vector < int > > modules_per_ladder_per_layer;

  void doLoops();
  void doLadderLoop();
  void doModuleLoop();

  int events;


     //  Int_t           layers;
  Short_t         ladder_p[LAYERS];   //[layers]
  Short_t         module_p[LAYERS];   //[layers]
  Short_t         segment_p[LAYERS];   //[layers]
  //  Int_t           sstrips[LAYERS];
  //  Int_t           HDSS_t[LAYERS][8];   //[layers]

  // List of branches
  //  TBranch        *b_layers;   //!
  TBranch        *b_ladder_p;   //!
  TBranch        *b_module_p;   //!
  TBranch        *b_segment_p;   //!
  //  TBranch        *b_sstrips;   //!
  //  TBranch        *b_HDSS_t;   //!
};

#endif
