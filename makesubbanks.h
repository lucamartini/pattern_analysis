#ifndef MAKESUBBANKS_H
#define MAKESUBBANKS_H

#include "patternloop.h"

#include "TH1D.h"

#include <iostream>
#include <vector>



class makesubbanks : public patternloop {
 public:
  makesubbanks(vector <int> * subbanks, vector < vector < vector < vector <int > > > > * subdetector_deg, string ttree_file = "inputs/treeBank.root", int nevent = -1, int ievent = 0);
  virtual ~makesubbanks();

  vector <int> * getSubbanks() {return subbanks_;};
  vector < vector < vector < vector <int > > > > * getSubdetector_deg() {return subdetector_deg_;};

  void doLoops();

  void setSubbankType(int type) {type_ = type;};
  void setSS5(int SS5) {SS5_ = SS5;};

  TH1D * get_SSsize_on_layer5(){return SSsize_on_layer5;};

 private:
  vector < vector < vector < vector <int > > > > * subdetector_deg_;
  vector <int> * subbanks_;
  int type_;
  int SS5_;
  void do2phi6_2z();
  void do2phi5_2z();
  void do4z();

  TH1D * SSsize_on_layer5;

};

#endif
