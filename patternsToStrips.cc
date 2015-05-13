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

int ladder7(int ladder_p) {
  if (ladder_p == 0) return 34;
  if (ladder_p >= 1 && ladder_p <= 5) return ladder_p;
  return -1;
}

int ladder9(int ladder_p) {
  if (ladder_p == 0) return 62;
  if (ladder_p >= 1 && ladder_p <= 9) return ladder_p;
  return -1;
}

int ladder10(int ladder_p) {
  if (ladder_p == 0) return 75;
  if (ladder_p == 1) return 76;
  if (ladder_p >= 2 && ladder_p <= 13) return ladder_p - 1;
  return -1;
}

int main(int argc, char **argv) {

  options opt(argc, argv);
  int ievent = opt.get_ievent();
  int nevent = opt.get_nevent();
  string tail = opt.get_tail();

  TFile StripCoords_f("inputs/CoordFiltered16.root"); //CoordFiltered extracted
  TTree * StripCoords_t = (TTree*)StripCoords_f.Get("StripCoords");
  Int_t           layer, module, ladder, strip, segment;
  Float_t         x, y, z;
  TBranch        *b_layer;   //!
  TBranch        *b_module;   //!
  TBranch        *b_ladder;   //!
  TBranch        *b_strip;   //!
  TBranch        *b_segment;   //!
  TBranch        *b_x;   //!
  TBranch        *b_y;   //!
  TBranch        *b_z;   //!

  StripCoords_t->SetBranchAddress("layer", &layer, &b_layer);
  StripCoords_t->SetBranchAddress("module", &module, &b_module);
  StripCoords_t->SetBranchAddress("ladder", &ladder, &b_ladder);
  StripCoords_t->SetBranchAddress("strip", &strip, &b_strip);
  StripCoords_t->SetBranchAddress("segment", &segment, &b_segment);
  StripCoords_t->SetBranchAddress("x", &x, &b_x);
  StripCoords_t->SetBranchAddress("y", &y, &b_y);
  StripCoords_t->SetBranchAddress("z", &z, &b_z);
  int sc_entries = StripCoords_t->GetEntries();

  TH2D * sc_xy_h = new TH2D("sc_xy_h", "sc_xy_h", 1000, -120., 120., 1000, -120., 120.);
  TH2D * sc_zr_h = new TH2D("sc_zr_h", "sc_zr_h", 1000, -270., 270., 1000, 0., 120.);


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

  TH2D * pat_xy_h = new TH2D("pat_xy_h", "pat_xy_h", 1000, -120., 120., 1000, -120., 120.);
  TH2D * pat_zr_h = new TH2D("pat_zr_h", "pat_zr_h", 1000, -270., 270., 1000, 0., 120.);

  for (int i = 0; i < sc_entries; i++) {
    if (i%1000000==0) cout << i << " / " << sc_entries << endl;
    StripCoords_t->GetEntry(i);
    float r = TMath::Sqrt(x*x + y*y);
    sc_xy_h->Fill(x, y);
    sc_zr_h->Fill(z, r);
  }
  plot * plot_ = new plot();
  plot_->setTail(tail);
  plot_->setDir("./plots_temp/");
  plot_->plot2D(sc_xy_h);
  plot_->plot2D(sc_zr_h);
  return 0;
  vector <int> subbanks = {0, 0, 0, 0}; // phi1z1, phi1z2, phi2z1, phi2z2

  vector <TH2D*> subpats_xy_h;
  vector <TH2D*> subpats_zr_h;
  for (int i = 0; i < 4; i++) {
    TH2D * subpat_xy_h = new TH2D(Form("subpat%d_xy_h", i), Form("subpat%d_xy_h", i), 1000, -120., 120., 1000, -120., 120.);
    subpats_xy_h.push_back(subpat_xy_h);
    TH2D * subpat_zr_h = new TH2D(Form("subpat%d_zr_h", i), Form("subpat%d_zr_h", i), 1000, -270., 270., 1000, 0., 120.);
    subpats_zr_h.push_back(subpat_zr_h);
  }

  TFile * outfile = new TFile(Form("./outputs/patternNstrips%s.root", tail.c_str()), "RECREATE");
  const int STRIP_SIZE(1024);

  if (nevent == -1) nevent = patterns_entries;
  int max_entry = min(ievent + nevent, patterns_entries);

  for (int j = ievent; j < max_entry; j++) {
    int l5 = 0, l6 = 0, l7 = 0, l8 = 0, l9 = 0, l10 = 0;

    patterns_t->GetEntry(j);

    if (j%1000==0) cout << j << " / " << max_entry << endl;
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

    bool found = false;
    for (int i = 0; i < sc_entries /* && found == false */; i++) {
      if (l5 > STRIP_SIZE || l6 > STRIP_SIZE || l7 > STRIP_SIZE || l8 > STRIP_SIZE || l9 > STRIP_SIZE || l10 > STRIP_SIZE) cout << "sfondato" << endl;
      //StripCoords_t->GetEntry(i);
      b_layer->GetEntry(i);
      if (layer == 5) {
        b_ladder->GetEntry(i);
        if (ladder == ladder_p[0] + 1) {
          b_module->GetEntry(i);
          if ((module - 24)/2 == module_p[0]) {
            b_segment->GetEntry(i);
            if ( (segment/16) == segment_p[0]) {
              b_strip->GetEntry(i);
              int superstrip_position = strip/32;
              for (int k = 0; k < sstrips[0]; k++) {
                if (superstrip_position == HDSS_t[0][k]) {
                  b_x->GetEntry(i);
                  b_y->GetEntry(i);
                  b_z->GetEntry(i);
                  float r = TMath::Sqrt(x*x + y*y);
                  if (ladder_p[0] == 0) { // let's drow only ladder 0
                    pat_xy_h->Fill(x, y);
                    pat_zr_h->Fill(z, r);
                  }
                  subpats_xy_h.at(subbank)->Fill(x, y);
                  subpats_zr_h.at(subbank)->Fill(z, r);
                  found = true;
                  // break;
                }
              }
            }
          }
        }
      }
      if (layer == 6) {
        b_ladder->GetEntry(i);
        if (ladder == ladder_p[1] + 1) {
          b_module->GetEntry(i);
          if ((module - 18)/2 == module_p[1]) {
            b_segment->GetEntry(i);
            if ( (segment/16) == segment_p[1]) {
              b_strip->GetEntry(i);
              int superstrip_position = strip/32;
              for (int k = 0; k < sstrips[1]; k++) {
                if (superstrip_position == HDSS_t[1][k]) {
                  b_x->GetEntry(i);
                  b_y->GetEntry(i);
                  b_z->GetEntry(i);
                  float r = TMath::Sqrt(x*x + y*y);
                  if (ladder_p[1] == 0) { // let's draw only ladder 0
                    pat_xy_h->Fill(x, y);
                    pat_zr_h->Fill(z, r);
                  }
                  subpats_xy_h.at(subbank)->Fill(x, y);
                  subpats_zr_h.at(subbank)->Fill(z, r);
                  found = true;
                  // break;
                }
              }
            }
          }
        }
      }
      if (layer == 7) {
        b_ladder->GetEntry(i);
        if (ladder == ladder7(ladder_p[2])) {
          b_module->GetEntry(i);
          if ((module - 16)/2 == module_p[2]) {
            b_segment->GetEntry(i);
            if ( (segment/16) == segment_p[2]) {
              b_strip->GetEntry(i);
              int superstrip_position = strip/32;
              for (int k = 0; k < sstrips[2]; k++) {
                if (superstrip_position == HDSS_t[2][k]) {
                  b_x->GetEntry(i);
                  b_y->GetEntry(i);
                  b_z->GetEntry(i);
                  float r = TMath::Sqrt(x*x + y*y);
                  if (ladder_p[2] == 0) { // let's draw only ladder 0
                    pat_xy_h->Fill(x, y);
                    pat_zr_h->Fill(z, r);
                  }
                  subpats_xy_h.at(subbank)->Fill(x, y);
                  subpats_zr_h.at(subbank)->Fill(z, r);
                  found = true;
                  // break;
                }
              }
            }
          }
        }
      }
      if (layer == 8) {
        b_ladder->GetEntry(i);
        if (ladder == ladder_p[3] + 1) {
          b_module->GetEntry(i);
          if (module - 5 == module_p[3]) {
            b_segment->GetEntry(i);
            if ( segment == segment_p[3]) {
              b_strip->GetEntry(i);
              int superstrip_position = strip/32;
              for (int k = 0; k < sstrips[3]; k++) {
                if (superstrip_position == HDSS_t[3][k]) {
                  b_x->GetEntry(i);
                  b_y->GetEntry(i);
                  b_z->GetEntry(i);
                  float r = TMath::Sqrt(x*x + y*y);
                  if (ladder_p[3] == 0) { // let's draw only ladder 0
                    pat_xy_h->Fill(x, y);
                    pat_zr_h->Fill(z, r);
                  }
                  subpats_xy_h.at(subbank)->Fill(x, y);
                  subpats_zr_h.at(subbank)->Fill(z, r);
                  found = true;
                  // break;
                }
              }
            }
          }
        }
      }
      if (layer == 9) {
        b_ladder->GetEntry(i);
        if (ladder == ladder9(ladder_p[4])) {
          b_module->GetEntry(i);
          if (module - 4 == module_p[4]) {
            b_segment->GetEntry(i);
            if ( segment == segment_p[4]) {
              b_strip->GetEntry(i);
              int superstrip_position = strip/32;
              for (int k = 0; k < sstrips[4]; k++) {
                if (superstrip_position == HDSS_t[4][k]) {
                  b_x->GetEntry(i);
                  b_y->GetEntry(i);
                  b_z->GetEntry(i);
                  float r = TMath::Sqrt(x*x + y*y);
                  if (ladder_p[4] == 0) { // let's draw only ladder 0
                    pat_xy_h->Fill(x, y);
                    pat_zr_h->Fill(z, r);
                  }
                  subpats_xy_h.at(subbank)->Fill(x, y);
                  subpats_zr_h.at(subbank)->Fill(z, r);
                  found = true;
                  // break;
                }
              }
            }
          }
        }
      }

      if (layer == 10) {
        b_ladder->GetEntry(i);
        if (ladder == ladder10(ladder_p[5])) {
          b_module->GetEntry(i);
          if (module - 2 == module_p[5]) {
            b_segment->GetEntry(i);
            if ( segment == segment_p[5]) {
              b_strip->GetEntry(i);
              int superstrip_position = strip/32;
              for (int k = 0; k < sstrips[5]; k++) {
                if (superstrip_position == HDSS_t[5][k]) {
                  b_x->GetEntry(i);
                  b_y->GetEntry(i);
                  b_z->GetEntry(i);
                  float r = TMath::Sqrt(x*x + y*y);
                  if (ladder_p[5] == 0) { // let's draw only ladder 0
                    pat_xy_h->Fill(x, y);
                    pat_zr_h->Fill(z, r);
                  }
                  subpats_xy_h.at(subbank)->Fill(x, y);
                  subpats_zr_h.at(subbank)->Fill(z, r);
                  found = true;
                  // break;
                }
              }
            }
          }
        }
      }
    }

    if (!found) cout << "not found: " << j << endl;

    if (j%10000==0) {
      plot_->plot2D(pat_xy_h);
      plot_->plot2D(pat_zr_h);
    }

  }

  int sum(0);
  for (int i = 0; i < 4; i++) {
    sum += subbanks[i];
    plot_->plot2D(subpats_xy_h.at(i));
    plot_->plot2D(subpats_zr_h.at(i));
  }
  plot_->plot2D(pat_xy_h);
  plot_->plot2D(pat_zr_h);

  //  plot(sstrips_size_h);

  ofstream subbankFile(Form("./subbanks/subbank%s.txt", tail.c_str()));
  cout << "in total, the bank is made by #patterns = " << patterns_entries << " ( " << sum << " ) " << endl;
  cout << "the 4 subbanks are made by:" << endl;
  for (int i = 0; i < 4; i++) {
    cout << subbanks[i] << endl;
    subbankFile << subbanks[i] << endl;
  }
  subbankFile.close();

  outfile->Close();
  return EXIT_SUCCESS;
}
