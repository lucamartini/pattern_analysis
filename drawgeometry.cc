#include "options.h"

#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TMath.h"

#include <iostream>

using namespace std;


int main(int argc, char **argv) {

  options opt(argc, argv);
  int ievent = opt.get_ievent();
  int nevent = opt.get_nevent();
  string tail = opt.get_tail();
  bool makeTree = opt.make_tree();

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
  TH2D * sc_xy0_h = new TH2D("sc_xy0_h", "sc_xy0_h", 1000, -120., 120., 1000, -120., 120.);
  sc_xy0_h->SetMarkerColor(kRed);
  TH2D * sc_zr_h = new TH2D("sc_zr_h", "sc_zr_h", 1000, -270., 270., 1000, 0., 120.);
  TH2D * sc_zr0_h = new TH2D("sc_zr0_h", "sc_zr0_h", 1000, -270., 270., 1000, 0., 120.);
  sc_zr0_h->SetMarkerColor(kRed);


  for (int i = 0; i < sc_entries; i++) {
    if (i%1000000==0) cout << i << " / " << sc_entries << endl;
    StripCoords_t->GetEntry(i);
    float r = TMath::Sqrt(x*x + y*y);
    sc_xy_h->Fill(x, y);
    sc_zr_h->Fill(z, r);
    if ( (layer == 8 && ladder == 1 && module == 12) ||
         (layer == 8 && ladder == 3 && module == 12) ||
         (layer == 8 && ladder == 5 && module == 12) ||
         (layer == 8 && ladder == 7 && module == 12) ||
         (layer == 10 && ladder == 76 && module == 11) ||
         (layer == 10 && ladder == 2 && module == 11) ||
         (layer == 10 && ladder == 4 && module == 11) ||
         (layer == 10 && ladder == 6 && module == 11) ||
         (layer == 10 && ladder == 8 && module == 11) ||
         (layer == 10 && ladder == 10 && module == 11) ||
         (layer == 10 && ladder == 12 && module == 11) ) {
      sc_xy0_h->Fill(x, y);
      sc_zr0_h->Fill(z, r);
    }
  }

  TCanvas * xy0_c = new TCanvas("xy0_c", "xy0_c", 600, 600);
  sc_xy_h->SetStats(0);
  sc_xy_h->Draw();
  sc_xy0_h->Draw("same");
  xy0_c->Print("./plots/sc_xy0_h.pdf");

  TCanvas * xy_c = new TCanvas("xy_c", "xy_c", 600, 600);
  sc_xy_h->Draw();
  xy_c->Print("./plots/sc_xy_h.pdf");

  TCanvas * zr0_c = new TCanvas("zr0_c", "zr0_c", 600, 600);
  sc_zr_h->SetStats(0);
  sc_zr_h->Draw();
  sc_zr0_h->Draw("same");
  zr0_c->Print("./plots/sc_zr0_h.pdf");

  TCanvas * zr_c = new TCanvas("zr_c", "zr_c", 600, 600);
  sc_zr_h->Draw();
  zr_c->Print("./plots/sc_zr_h.pdf");


  return EXIT_SUCCESS;
}
