#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <getopt.h>

using namespace std;

const char* program_name;
char* tail = "";

void print_usage (FILE* stream, int exit_code) {
  fprintf (stream, "Usage: %s options [ ... ]\n", program_name);
  fprintf (stream,
           " -h --help Display this usage information.\n"
           " -i --ievent Start from event i.\n"
           " -n --nevent Run for n events.\n"
           " -t --tail Append tail to output files.\n");
  exit (exit_code);
}

void plot(TH1D & h) {
  TCanvas c("c", "c", 600, 600);
  h.Draw();
  c.Print(Form("./plots_temp/%s%s.pdf", h.GetName(), tail));
  h.SaveAs(Form("./plots_temp/%s%s.root", h.GetName(), tail));
}

void plot_2D(TH2D & h) {

  TCanvas c("c", "c", 600, 600);

  h.SetStats(0);
  h.Draw("COLZ");
  c.Print(Form("./plots_temp/%s%s.pdf", h.GetName(), tail));
  h.SaveAs(Form("./plots_temp/%s%s.root", h.GetName(), tail));
}

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

int main(int argc, char *argv[]) {

  int next_option;
  const char* const short_options = "hi:n:t:";

  const struct option long_options[] = {
    { "help", 0, NULL, 'h' },
    { "ievent", 1, NULL, 'i' },
    { "nevent", 1, NULL, 'n' },
    { "tail", 1, NULL, 't' },
    { NULL, 0, NULL, 0 } /* Required at end of array. */
  };

  int ievent = 0;
  int nevent = -1;
  program_name = argv[0];

  do {
    next_option = getopt_long (argc, argv, short_options, long_options, NULL);
    switch (next_option)
      {
      case 'h':
        /* User has requested usage information. Print it to standard output, and exit with exit code zero (normal termination). */
        print_usage (stdout, 0);

      case 'i':
        ievent = atoi(optarg);
        cout << "starting from event " << ievent << endl;
        break;

      case 'n':
        nevent = atoi(optarg);
        cout << "running for " << nevent << " events" << endl;
        break;

      case 't':
        tail = optarg;
        cout << "appending " << tail << endl;
        break;

      case '?': /* The user specified an invalid option. */
        /* Print usage information to standard error, and exit with exit code one (indicating abnormal termination). */
        print_usage (stderr, 1);

      case -1:  /* Done with options. */
        break;

      default: /* Something else: unexpected. */
        abort ();
      }
  }
  while (next_option != -1);

  TFile StripCoords_f("inputs/CoordFiltered16_layer5.root"); //CoordFiltered extracted
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
  StripCoords_t->SetBranchStatus("layer", false);

  StripCoords_t->SetBranchAddress("module", &module, &b_module);
  StripCoords_t->SetBranchAddress("ladder", &ladder, &b_ladder);
  StripCoords_t->SetBranchAddress("strip", &strip, &b_strip);
  StripCoords_t->SetBranchAddress("segment", &segment, &b_segment);
  StripCoords_t->SetBranchAddress("x", &x, &b_x);
  StripCoords_t->SetBranchAddress("y", &y, &b_y);
  StripCoords_t->SetBranchAddress("z", &z, &b_z);
  int sc_entries = StripCoords_t->GetEntries();

  TH2D sc_xy_h("sc_xy_h", "sc_xy_h", 1000, -120., 120., 1000, -120., 120.);
  TH2D sc_zr_h("sc_zr_h", "sc_zr_h", 1000, -270., 270., 1000, 0., 120.);


  TFile patterns_f("./inputs/treeBank.root");
  TTree * patterns_t = (TTree*)patterns_f.Get("sector0");

  Int_t           layers;
  Short_t         ladder_t[6];   //[layers]
  Short_t         module_t[6];   //[layers]
  Short_t         segment_t[6];   //[layers]

   Int_t           sstrips[6];
   Int_t           HDSS_t[6][8];   //[layers]

   // List of branches
  TBranch        *b_layers;   //!
  TBranch        *b_ladder_t;   //!
  TBranch        *b_module_t;   //!
  TBranch        *b_segment_t;   //!
  TBranch        *b_sstrips;   //!
  TBranch        *b_HDSS_t;   //!
  patterns_t->SetBranchAddress("layers", &layers, &b_layers);
  patterns_t->SetBranchAddress("ladder_t", ladder_t, &b_ladder_t);
  patterns_t->SetBranchAddress("module_t", module_t, &b_module_t);
  patterns_t->SetBranchAddress("segment_t", segment_t, &b_segment_t);
  patterns_t->SetBranchAddress("sstrips", &sstrips, &b_sstrips);
  patterns_t->SetBranchAddress("HDSS_t", HDSS_t, &b_HDSS_t);


  int patterns_entries = patterns_t->GetEntries();

  cout << "patterns are " << patterns_entries << endl;

  TH2D pat_xy_h("pat_xy_h", "pat_xy_h", 1000, -120., 120., 1000, -120., 120.);
  TH2D pat_zr_h("pat_zr_h", "pat_zr_h", 1000, -270., 270., 1000, 0., 120.);

  for (int i = 0; i < sc_entries; i++) {
    if (i%1000000==0) cout << i << " / " << sc_entries << endl;
    StripCoords_t->GetEntry(i);
    float r = TMath::Sqrt(x*x + y*y);
    sc_xy_h.Fill(x, y);
    sc_zr_h.Fill(z, r);
  }
  plot_2D(sc_xy_h);
  plot_2D(sc_zr_h);

  vector <int> subbanks = {0, 0, 0, 0}; // phi1z1, phi1z2, phi2z1, phi2z2

  // TH1D sstrips_size_h("sstrips_size_h", "sstrips_size_h", 9, 0, 9);
  // TH1D sstrips_size_l5_h("sstrips_size_l5_h", "sstrips_size_l5_h", 9, 0, 9);

  if (nevent == -1) nevent = patterns_entries;
  for (int j = ievent; j < ievent + nevent && j < patterns_entries; j++) {
    patterns_t->GetEntry(j);

    if (j%1000==0) cout << j << " / " << patterns_entries << endl;

    // bool found = false;

    for (int i = 0; i < sc_entries /* && found == false */; i++) {
      //StripCoords_t->GetEntry(i);
      b_layer->GetEntry(i);
      //   if (layer == 5) {
        b_ladder->GetEntry(i);
        if ( (ladder - 1) == ladder_t[0]) {
          b_module->GetEntry(i);
          if ((module - 24)/2 == module_t[0]) {
            b_segment->GetEntry(i);
            if ( (segment/16) == segment_t[0]) {
              b_strip->GetEntry(i);
              int superstrip_position = strip/32;
              for (int k = 0; k < sstrips[0]; k++) {
                if (superstrip_position == HDSS_t[0][k]) {
                  b_x->GetEntry(i);
                  b_y->GetEntry(i);
                  b_z->GetEntry(i);
                  float r = TMath::Sqrt(x*x + y*y);
                  pat_xy_h.Fill(x, y);
                  pat_zr_h.Fill(z, r);
                  // found = true;
                  // break;
                }
              }
            }
          }
          //   }
      }
      //else break;
      //cout << "not found" << endl;
    }
    // if (found == false) cout << "not found" << endl;

    if (ladder_t[1] < 2 && module_t[5] < 5) {
      subbanks[0]++;
    }
    if (ladder_t[1] < 2 && module_t[5] >= 5) {
      subbanks[1]++;
    }
    if (ladder_t[1] >= 2 && module_t[5] < 5) {
      subbanks[2]++;
    }
    if (ladder_t[1] >= 2 && module_t[5] >= 5) {
      subbanks[3]++;
    }


    if (j%10000==0) {
      plot_2D(pat_xy_h);
      plot_2D(pat_zr_h);
    }

  }

  int sum(0);
  for (int i = 0; i < 4; i++) {
    sum += subbanks[i];
  }

  plot_2D(pat_xy_h);
  plot_2D(pat_zr_h);

  //  plot(sstrips_size_h);

  ofstream subbankFile(Form("./subbanks/subbank%s.txt", tail));
  cout << "in total, the bank is made by #patterns = " << patterns_entries << " ( " << sum << " ) " << endl;
  cout << "the 4 subbanks are made by:" << endl;
  for (int i = 0; i < 4; i++) {
    cout << subbanks[i] << endl;
    subbankFile << subbanks[i] << endl;
  }
  subbankFile.close();

  return EXIT_SUCCESS;
}
