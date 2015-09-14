#ifndef PATTERNLOOP_H
#define PATTERNLOOP_H

#include "TFile.h"
#include "TTree.h"

//#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

using namespace std;

const unsigned int LAYERS = 6;

class patternloop {
 public:
  patternloop(string ttree_file = "inputs/treeBank.root", int nevent = -1, int ievent = 0);
  virtual ~patternloop();

  int get_entries() {return events;};
  virtual void doLoops() = 0;

 protected:
  TFile * input_f;
  TTree * tree;

  int events;
  int nevent_;
  int ievent_;
  int max_entry;

     //  Int_t           layers;
  Short_t         ladder_p[LAYERS];   //[layers]
  Short_t         module_p[LAYERS];   //[layers]
  Short_t         segment_p[LAYERS];   //[layers]
  Int_t           sstrips[LAYERS];
  Int_t           HDSS_t[LAYERS][8];   //[layers]

  // List of branches
  //  TBranch        *b_layers;   //!
  TBranch        *b_ladder_p;   //!
  TBranch        *b_module_p;   //!
  TBranch        *b_segment_p;   //!
  TBranch        *b_sstrips;   //!
  TBranch        *b_HDSS_t;   //!

  int sector;
};

#endif
