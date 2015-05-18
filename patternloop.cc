#include "patternloop.h"

patternloop::patternloop(string ttree_file, int nevent, int ievent) : nevent_(nevent), ievent_(ievent) {
  input_f = new TFile(ttree_file.c_str());
  tree = (TTree*)input_f->Get("sector0");

  events = tree->GetEntries();
  if (nevent == -1) nevent = events;
  max_entry = min(ievent + nevent, events);


  // tree->SetBranchAddress("layers", &layers, &b_layers);
  tree->SetBranchStatus("layers", false);
  tree->SetBranchAddress("ladder_t", ladder_p, &b_ladder_p);
  tree->SetBranchAddress("module_t", module_p, &b_module_p);
  tree->SetBranchAddress("segment_t", segment_p, &b_segment_p);
  tree->SetBranchAddress("sstrips", &sstrips, &b_sstrips);
  tree->SetBranchAddress("HDSS_t", HDSS_t, &b_HDSS_t);
  // tree->SetBranchStatus("sstrips", false);
  // tree->SetBranchStatus("HDSS_t", false);

  //  doLoops();

}

patternloop::~patternloop() {
  if (input_f) input_f->Close();
}
