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
  string inputfile = opt.get_file();
  int SS5 = opt.get_SS5();

  TFile patterns_f(inputfile.c_str());
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
  // patterns_t->SetBranchStatus("sstrips", false);
  // patterns_t->SetBranchStatus("HDSS_t", false);

  int patterns_entries = patterns_t->GetEntries();

  cout << "patterns are " << patterns_entries << endl;

  plot * plot_ = new plot();
  plot_->setTail(tail);
  plot_->setDir("./plots_temp/");

  vector <int> subbanks = {0, 0, 0, 0, 0}; // phi1z1, phi1z2, phi2z1, phi2z2, phi12

  int count = 0;
  getModuleSizes * gms = new getModuleSizes(inputfile.c_str());
  vector < vector < vector < int > > > allModules = gms->getLayerLadderModule();
  vector < vector < vector < vector <int > > > > subdetector_deg;
  subdetector_deg.resize(allModules.size()); // layer
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) {
    if (i == 0) subdetector_deg.at(i).resize(allModules.at(i).size()-1); // ladder 0: 1 + 0.5
    else subdetector_deg.at(i).resize(allModules.at(i).size()); // ladder
    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++) {
      subdetector_deg.at(i).at(j).resize(allModules.at(i).at(j).size()); // module

      for (unsigned int k = 0; k < subdetector_deg.at(i).at(j).size(); k++) {
        subdetector_deg.at(i).at(j).at(k).resize(subbanks.size()); // subbanks
        count++;
        for (unsigned int l = 0; l < subdetector_deg.at(i).at(j).at(k).size(); l++) {
          subdetector_deg.at(i).at(j).at(k).at(l) = 0;
        }
      }
    }
  }
  cout << "there are " << count << " subdetectors" << endl;

  int strip0_MIN = 0;
  int strip0_MAX = (1024/SS5) - 1;
  int strip0_MID = strip0_MAX / 2;

  TH1D * SSsize_on_layer5 = new TH1D("SSsize_on_layer5", Form("SSsize_on_layer5 (#times %d)", SS5), 10, 0., 10.);

  if (nevent == -1) nevent = patterns_entries;
  int max_entry = min(ievent + nevent, patterns_entries);

  for (int i = ievent; i < max_entry; i++) {

    patterns_t->GetEntry(i);
    int subbank = -1;
    if (i%100000==0) cout << i << " / " << max_entry << endl;
    SSsize_on_layer5->Fill(sstrips[0]);
    // strips = 960
    // half = 480
    int min_strip = 1000, max_strip = -1;
    if (ladder_p[0] == 1) {
      for (int j = 0; j < sstrips[0]; j++) {
        if (HDSS_t[0][j] < min_strip) {
          min_strip = HDSS_t[0][j];
        }
        if (HDSS_t[0][j] > max_strip) {
          max_strip = HDSS_t[0][j];
        }
      }
      if (min_strip < strip0_MIN) cout << "min_strip " << min_strip << endl;
      if (max_strip > strip0_MAX) cout << "max_strip " << max_strip << endl;
      if (min_strip > max_strip) cout << "WTF" << endl;
    }

    bool first_half_in_ladder1 =  (ladder_p[0] == 1 && max_strip <= strip0_MID);
    bool first_half_in_ladders = (ladder_p[0] == 0) || first_half_in_ladder1;

    if (ladder_p[0] == 1 && min_strip <= strip0_MID && max_strip >= strip0_MID + 1) {
      subbanks[4]++;
      subbank = 4;
      cout << min_strip << " " << max_strip << endl; // 0 by construction
    }
    else {
      if (first_half_in_ladders && module_p[5] < 5) {
        subbanks[0]++;
        subbank = 0;
      }
      if (first_half_in_ladders && module_p[5] >= 5) {
        subbanks[1]++;
        subbank = 1;
      }
      if (!first_half_in_ladders && module_p[5] < 5) {
        subbanks[2]++;
        subbank = 2;
      }
      if (!first_half_in_ladders && module_p[5] >= 5) {
        subbanks[3]++;
        subbank = 3;
      }
    }

    for (int j = 0; j <= 5; j++) {
      //      if (j == 0 && ladder_p[0] == 1) continue;
      //subdetector_deg.at(j).at(ladder_p[j]).at(module_p[j]).at(subbank)++; // subdetector is in subbank
      unsigned int ladderino = ladder_p[j];
      if (j == 0 && ladder_p[0] == 1) {
        if (first_half_in_ladder1) ladderino = 0;
        else ladderino = 1;
      }
      else if (j == 0 && ladder_p[0] == 2) ladderino = 1;

      subdetector_deg.at(j).at(ladderino).at(module_p[j]).at(subbank)++; // subdetector is in subbank
    }
  }
  cout << "pattern loop ended" << endl;

  int sum(0);
  for (unsigned int i = 0; i < subbanks.size(); i++) {
    sum += subbanks[i];
  }

  ofstream subbankFile(Form("./subbanks/subbank%s.txt", tail.c_str()));
  cout << "in total, the bank is made by #patterns = " << patterns_entries << " ( " << sum << " ) " << endl;
  cout << "the 4 + 1 subbanks are made by:" << endl;
  for (unsigned int i = 0; i < subbanks.size(); i++) {
    cout << subbanks[i] << endl;
    subbankFile << subbanks[i] << endl;
  }
  subbankFile.close();

  TH1D * subdetector_deg_h = new TH1D("subdetector_deg_h", Form("subdetector_deg_h%s;degeneration;number of modules", tail.c_str()), 5, 0, 5);
  subdetector_deg_h->SetFillColor(kBlue);
  subdetector_deg_h->SetFillStyle(3001);
  vector <TH1D *> subdetectors_deg_layer_h;

  struct jk {
    int j;
    int k;
  };

  vector <jk> jk_v;

  cout << "subdetectors not contained in any patterns are :" << endl;
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) { // for each layer
    TH1D * subdetector_deg_layer_h = new TH1D(Form("subdetector_deg_h_%d", i), Form("subdetector_deg_h_%d%s;degeneration;number of modules", i, tail.c_str()), 5, 0, 5);
    subdetector_deg_layer_h->SetFillColor(kBlue);
    subdetector_deg_layer_h->SetFillStyle(3001);
    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++) { // for each ladder
      for (unsigned int k = 0; k < subdetector_deg.at(i).at(j).size(); k++) { // for each module
        int deg = 0;
        for (unsigned int l = 0; l < subdetector_deg.at(i).at(j).at(k).size()-1; l++) { // for each real subbbank
          if (subdetector_deg.at(i).at(j).at(k).at(l) > 0) {
            deg++;
          }
        }
        if (deg == 0) cout << i << " " << j << " " << k << endl;
        subdetector_deg_h->Fill(deg);
        subdetector_deg_layer_h->Fill(deg);
        if (i == 0 && deg > 2) {
          jk jk0;
          jk0.j = j;
          jk0.k = k;
          jk_v.push_back(jk0);
        }
      }
    }
    subdetectors_deg_layer_h.push_back(subdetector_deg_layer_h);
  }

  plot * deg_p = new plot();
  deg_p->setDir("./plots/");
  deg_p->setTail(tail);
  deg_p->plot1D(subdetector_deg_h);
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) { // for each layer
    deg_p->plot1D(subdetectors_deg_layer_h.at(i));
  }
  deg_p->plot1D(SSsize_on_layer5);

  cout << "these fking deg in layer 0 (rightly) are: " << endl;
  for (unsigned i = 0; i < jk_v.size(); i++) {
    cout << jk_v.at(i).j << "  " << jk_v.at(i).k << endl;
  }

  TH1D * subbanks_h = new TH1D("subbanks_h", Form("subbanks_h%s;subbanks;fraction of modules", tail.c_str()), 4, 0, 4);
  subbanks_h->GetXaxis()->SetBinLabel(1, "#phi_{1}Z_{1}");
  subbanks_h->GetXaxis()->SetBinLabel(2, "#phi_{1}Z_{2}");
  subbanks_h->GetXaxis()->SetBinLabel(3, "#phi_{2}Z_{1}");
  subbanks_h->GetXaxis()->SetBinLabel(4, "#phi_{2}Z_{2}");
  subbanks_h->GetYaxis()->SetTitleOffset(1.2);
  subbanks_h->SetMinimum(0);
  subbanks_h->SetStats(0);
  subbanks_h->SetLineWidth(2);

  vector <int> ladder_module_integral(subdetector_deg.at(0).at(0).at(0).size(), 0);

  vector <TH1D *> subbanks_h_v;
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) { // for each layer
    TH1D * subbanks_layer_h = new TH1D(Form("subbanks_layer_h_%d", i), Form("subbanks_layer_h_%d%s;subbanks;fraction of modules", i, tail.c_str()), 4, 0, 4);
    subbanks_layer_h->GetXaxis()->SetBinLabel(1, "#phi_{1}Z_{1}");
    subbanks_layer_h->GetXaxis()->SetBinLabel(2, "#phi_{1}Z_{2}");
    subbanks_layer_h->GetXaxis()->SetBinLabel(3, "#phi_{2}Z_{1}");
    subbanks_layer_h->GetXaxis()->SetBinLabel(4, "#phi_{2}Z_{2}");
    subbanks_layer_h->GetYaxis()->SetTitleOffset(1.2);
    subbanks_layer_h->SetMinimum(0);
    subbanks_layer_h->SetStats(0);
    subbanks_layer_h->SetLineWidth(2);
    vector <int> ladder_module_layer_integral(subdetector_deg.at(i).at(0).at(0).size(), 0);

    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++) { // for each ladder
      for (unsigned int k = 0; k < subdetector_deg.at(i).at(j).size(); k++) { // for each module
        for (unsigned int l = 0; l < subdetector_deg.at(i).at(j).at(k).size(); l++) { // for each subbbank
          if (subdetector_deg.at(i).at(j).at(k).at(l)>0) {
            //          ladder_module_layer_integral[l] +=  subdetector_deg.at(i).at(j).at(k).at(l);
            //          ladder_module_integral[l] += subdetector_deg.at(i).at(j).at(k).at(l);
            ladder_module_layer_integral.at(l)++;
            ladder_module_integral.at(l)++;
          }
        }
      }
    }
    for (unsigned int l = 0; l < subdetector_deg.at(i).at(0).at(0).size(); l++) {
      subbanks_layer_h->SetBinContent(l+1, (double) ladder_module_layer_integral.at(l) / subdetectors_deg_layer_h.at(i)->Integral());
    }
    subbanks_h_v.push_back(subbanks_layer_h);
  }
  for (unsigned int l = 0; l < subdetector_deg.at(0).at(0).at(0).size(); l++) {
    subbanks_h->SetBinContent(l+1, (double) ladder_module_integral.at(l)/subdetector_deg_h->Integral());
  }

  deg_p->plot1D(subbanks_h);
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) { // for each layer
    deg_p->plot1D(subbanks_h_v.at(i));
  }

  return EXIT_SUCCESS;
}
