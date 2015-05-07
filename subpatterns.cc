#include "options.h"
#include "plot.h"
#include "getModuleSizes.h"

#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using namespace std;

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
  //  Int_t           sstrips[6];
  //  Int_t           HDSS_t[6][8];   //[layers]

  // List of branches
  //  TBranch        *b_layers;   //!
  TBranch        *b_ladder_p;   //!
  TBranch        *b_module_p;   //!
  TBranch        *b_segment_p;   //!
  //  TBranch        *b_sstrips;   //!
  //  TBranch        *b_HDSS_t;   //!
  // patterns_t->SetBranchAddress("layers", &layers, &b_layers);
  patterns_t->SetBranchStatus("layers", false);
  patterns_t->SetBranchAddress("ladder_t", ladder_p, &b_ladder_p);
  patterns_t->SetBranchAddress("module_t", module_p, &b_module_p);
  patterns_t->SetBranchAddress("segment_t", segment_p, &b_segment_p);
  //patterns_t->SetBranchAddress("sstrips", &sstrips, &b_sstrips);
  //patterns_t->SetBranchAddress("HDSS_t", HDSS_t, &b_HDSS_t);
  patterns_t->SetBranchStatus("sstrips", false);
  patterns_t->SetBranchStatus("HDSS_t", false);

  int patterns_entries = patterns_t->GetEntries();

  cout << "patterns are " << patterns_entries << endl;

  plot * plot_ = new plot();
  plot_->setTail(tail);
  plot_->setDir("./plots_temp/");

  vector <int> subbanks = {0, 0, 0, 0}; // phi1z1, phi1z2, phi2z1, phi2z2
  TH1D * subbanks_h = new TH1D("subbanks_h", "subbanks_h", 4, 0, 4);

  // layer, ladder #, subbank
  vector < vector < vector < int > > > ladder_deg;

  ladder_deg.resize(6);
  ladder_deg.at(0).resize(3);
  ladder_deg.at(1).resize(4);
  ladder_deg.at(2).resize(6);
  ladder_deg.at(3).resize(7);
  ladder_deg.at(4).resize(10);
  ladder_deg.at(5).resize(14);
  for (unsigned int i = 0; i < ladder_deg.size(); i++) {
    for (unsigned int j = 0; j < ladder_deg.at(i).size(); j++) {
      ladder_deg.at(i).at(j).resize(4);
      for (unsigned int k = 0; k < ladder_deg.at(i).at(j).size(); k++) {
        ladder_deg.at(i).at(j).at(k) = 0;
      }
    }
  }

  // layer, module #, subbank
  vector < vector < vector < int > > > module_deg;
  module_deg.resize(6);
  module_deg.at(0).resize(5);
  module_deg.at(1).resize(6);
  module_deg.at(2).resize(7);
  module_deg.at(3).resize(8);
  module_deg.at(4).resize(9);
  module_deg.at(5).resize(10);
  for (unsigned int i = 0; i < module_deg.size(); i++) {
    for (unsigned int j = 0; j < module_deg.at(i).size(); j++) {
      module_deg.at(i).at(j).resize(4);
      for (unsigned int k = 0; k < module_deg.at(i).at(j).size(); k++) {
        module_deg.at(i).at(j).at(k) = 0;
      }
    }
  }

  /*
  // layer, ladder, module, subbank
  vector < vector < vector < vector <int > > > > subdetector_deg(6); // layers
  subdetector_deg.at(0).resize(3); // ladders: 1 + 0.5
  subdetector_deg.at(1).resize(4);
  subdetector_deg.at(2).resize(6);
  subdetector_deg.at(3).resize(7);
  subdetector_deg.at(4).resize(10);
  subdetector_deg.at(5).resize(14);
  int cont = 0;
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) {
    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++) {

      // subdetector_deg.at(i).at(j).resize(5 + i); // modules

      if (i <= 2 || i == 4) {
        subdetector_deg.at(i).at(j).resize(5 + i);
      }
      else if (i == 3) {
        if (j%2==0) subdetector_deg.at(i).at(j).resize(4 + i);
        else subdetector_deg.at(i).at(j).resize(5 + i);
      }
      else if (i == 5)  {
        if (j%2==0) subdetector_deg.at(i).at(j).resize(5 + i);
        else subdetector_deg.at(i).at(j).resize(4 + i);
      }

      for (unsigned int k = 0; k < subdetector_deg.at(i).at(j).size(); k++) {
        subdetector_deg.at(i).at(j).at(k).resize(subbanks.size()); // subbanks
        cont++;
        for (unsigned int l = 0; l < subdetector_deg.at(i).at(j).at(k).size(); l++) {
          subdetector_deg.at(i).at(j).at(k).at(l) = 0;

        }
      }
    }
  }
  cout << "there are " << cont << " subdetectors" << endl;
  */

  ///*
  int count = 0;
  getModuleSizes * gms = new getModuleSizes;
  vector < vector < vector < int > > > allModules = gms->getLayerLadderModule();
  vector < vector < vector < vector <int > > > > subdetector_deg;
  subdetector_deg.resize(allModules.size()); // layer
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) {
    subdetector_deg.at(i).resize(allModules.at(i).size()); // ladder
    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++) {
      subdetector_deg.at(i).at(j).resize(allModules.at(i).at(j).size()); // module

      for (unsigned int k = 0; k < subdetector_deg.at(i).at(j).size(); k++) {
        subdetector_deg.at(i).at(j).at(k).resize(4); // subbanks
        count++;
        for (unsigned int l = 0; l < subdetector_deg.at(i).at(j).at(k).size(); l++) {
          subdetector_deg.at(i).at(j).at(k).at(l) = 0;
        }
      }
    }
  }
  cout << "there are " << count << " subdetectors" << endl;
  //*/

  if (nevent == -1) nevent = patterns_entries;
  int max_entry = min(ievent + nevent, patterns_entries);

  for (int i = ievent; i < max_entry; i++) {

    patterns_t->GetEntry(i);
    int subbank = -1;
    if (i%100000==0) cout << i << " / " << max_entry << endl;
    if (ladder_p[1] < 2 && module_p[5] < 5) {
      subbanks[0]++;
      subbanks_h->Fill(0);
      subbank = 0;
    }
    if (ladder_p[1] < 2 && module_p[5] >= 5) {
      subbanks[1]++;
      subbanks_h->Fill(1);
      subbank = 1;
    }
    if (ladder_p[1] >= 2 && module_p[5] < 5) {
      subbanks[2]++;
      subbanks_h->Fill(2);
      subbank = 2;
    }
    if (ladder_p[1] >= 2 && module_p[5] >= 5) {
      subbanks[3]++;
      subbanks_h->Fill(3);
      subbank = 3;
    }

    for (int j = 0; j <= 5; j++) {
      ladder_deg.at(j).at(ladder_p[j]).at(subbank)++;
      module_deg.at(j).at(module_p[j]).at(subbank)++;
      subdetector_deg.at(j).at(ladder_p[j]).at(module_p[j]).at(subbank)++; // subdetector is in subbank

    }
  }
  cout << "pattern loop ended" << endl;

  int sum(0);
  for (int i = 0; i < 4; i++) {
    sum += subbanks[i];
  }

  ofstream subbankFile(Form("./subbanks/subbank%s.txt", tail.c_str()));
  cout << "in total, the bank is made by #patterns = " << patterns_entries << " ( " << sum << " ) " << endl;
  cout << "the 4 subbanks are made by:" << endl;
  for (int i = 0; i < 4; i++) {
    cout << subbanks[i] << endl;
    subbankFile << subbanks[i] << endl;
  }
  subbankFile.close();

  TH1D * ladder_deg_h = new TH1D("ladder_deg_h", "ladder_deg_h", 5, 0, 5);
  TH1D * module_deg_h = new TH1D("module_deg_h", "module_deg_h", 5, 0, 5);

  for (unsigned int i = 0; i < ladder_deg.size(); i++) {
    for (unsigned int j = 0; j < ladder_deg.at(i).size(); j++) {
      int deg = 0;
      for (unsigned int k = 0; k < ladder_deg.at(i).at(j).size(); k++) {
        if (ladder_deg.at(i).at(j).at(k) > 0) {
          deg++;
        }
      }
      ladder_deg_h->Fill(deg);
    }
  }


  for (unsigned int i = 0; i < module_deg.size(); i++) {
    for (unsigned int j = 0; j < module_deg.at(i).size(); j++) {
      int deg = 0;
      for (unsigned int k = 0; k < module_deg.at(i).at(j).size(); k++) {
        if (module_deg.at(i).at(j).at(k) > 0) {
          deg++;
        }
      }
      module_deg_h->Fill(deg);
    }
  }


  TH1D * subdetector_deg_h = new TH1D("subdetector_deg_h", "subdetector_deg_h", 5, 0, 5);
  vector <TH1D *> subdetectors_deg_layer_h;

  cout << "subdetectors not contained in any patterns are :" << endl;
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) { // for each layer
      TH1D * subdetector_deg_layer_h = new TH1D(Form("subdetector_deg_h_%d", i), "subdetector_deg_h", 5, 0, 5);

    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++) { // for each ladder
      for (unsigned int k = 0; k < subdetector_deg.at(i).at(j).size(); k++) { // for each module
        int deg = 0;
        for (unsigned int l = 0; l < subdetector_deg.at(i).at(j).at(k).size(); l++) { // for each subbbank
          if (subdetector_deg.at(i).at(j).at(k).at(l) > 0) {
            deg++;
          }
        }
        if (deg == 0) cout << i << " " << j << " " << k << endl;
        subdetector_deg_h->Fill(deg);
        subdetector_deg_layer_h->Fill(deg);
      }
    }
    subdetectors_deg_layer_h.push_back(subdetector_deg_layer_h);
  }

  plot * deg_p = new plot();
  deg_p->setDir("./plots/");
  deg_p->setTail(tail);
  deg_p->plot1D(ladder_deg_h);
  deg_p->plot1D(module_deg_h);
  deg_p->plot1D(subdetector_deg_h);
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) { // for each layer
    deg_p->plot1D(subdetectors_deg_layer_h.at(i));
  }

  return EXIT_SUCCESS;
}
