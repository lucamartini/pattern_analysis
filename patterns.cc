#include "options.h"
#include "plot.h"

#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct strip {
  int layer;
  int module;
  int ladder;
  int strip;
  int segment;
};

void print_strip(struct strip * s) {
  cout << "strip " << s << "; layer = " << s->layer << "; module = " << s->module << "; ladder = " << s->ladder << "; strip = " << s->strip << "; segment = " << s->segment << endl;
}

struct coords {
  float x;
  float y;
  float z;
};

int main(int argc, char **argv) {

  options opt(argc, argv);
  int ievent = opt.get_ievent();
  int nevent = opt.get_nevent();
  string tail = opt.get_tail();

  TFile patterns_f("./inputs/treeBank.root");
  TTree * patterns_t = (TTree*)patterns_f.Get("sector0");

  //  Int_t           layers;
  Short_t         ladder_p[6];   //[layers]
  Short_t         module_p[6];   //[layers]
  Short_t         segment_p[6];   //[layers]

  Int_t           sstrips[6];
  Int_t           HDSS_t[6][8];   //[layers]

  // List of branches
  //  TBranch        *b_layers;   //!
  TBranch        *b_ladder_p;   //!
  TBranch        *b_module_p;   //!
  TBranch        *b_segment_p;   //!
  TBranch        *b_sstrips;   //!
  TBranch        *b_HDSS_t;   //!
  // patterns_t->SetBranchAddress("layers", &layers, &b_layers);
  patterns_t->SetBranchStatus("layers", false);
  patterns_t->SetBranchAddress("ladder_t", ladder_p, &b_ladder_p);
  patterns_t->SetBranchAddress("module_t", module_p, &b_module_p);
  patterns_t->SetBranchAddress("segment_t", segment_p, &b_segment_p);
  patterns_t->SetBranchAddress("sstrips", &sstrips, &b_sstrips);
  patterns_t->SetBranchAddress("HDSS_t", HDSS_t, &b_HDSS_t);

  int patterns_entries = patterns_t->GetEntries();

  cout << "patterns are " << patterns_entries << endl;

  plot * plot_ = new plot();
  plot_->setTail(tail);
  plot_->setDir("./plots_temp/");

  vector <int> subbanks = {0, 0, 0, 0}; // phi1z1, phi1z2, phi2z1, phi2z2

  if (nevent == -1) nevent = patterns_entries;
  int max_entry = min(ievent + nevent, patterns_entries);

  for (int j = ievent; j < max_entry; j++) {

    patterns_t->GetEntry(j);

    if (j%100000==0) cout << j << " / " << max_entry << endl;
    int subbank = -1;
    if (ladder_p[1] < 2 && module_p[5] < 5) {
      subbanks[0]++;
      subbank = 0;
    }
    if (ladder_p[1] < 2 && module_p[5] >= 5) {
      subbanks[1]++;
      subbank = 1;
    }
    if (ladder_p[1] >= 2 && module_p[5] < 5) {
      subbanks[2]++;
      subbank = 2;
    }
    if (ladder_p[1] >= 2 && module_p[5] >= 5) {
      subbanks[3]++;
      subbank = 3;
    }

  }

  int sum(0);
  for (int i = 0; i < 4; i++) {
    sum += subbanks[i];
  }

  //  plot(sstrips_size_h);

  ofstream subbankFile(Form("./subbanks/subbank%s.txt", tail.c_str()));
  cout << "in total, the bank is made by #patterns = " << patterns_entries << " ( " << sum << " ) " << endl;
  cout << "the 4 subbanks are made by:" << endl;
  for (int i = 0; i < 4; i++) {
    cout << subbanks[i] << endl;
    subbankFile << subbanks[i] << endl;
  }
  subbankFile.close();

  return EXIT_SUCCESS;
}
