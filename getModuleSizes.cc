#include "getModuleSizes.h"

getModuleSizes::getModuleSizes(string ttree_file) : stripsOn0(-1) {
  input_f = new TFile(ttree_file.c_str());
  tree = (TTree*)input_f->Get("sector0");

  events = tree->GetEntries();

  // tree->SetBranchAddress("layers", &layers, &b_layers);
  tree->SetBranchStatus("layers", false);
  tree->SetBranchAddress("ladder_t", ladder_p, &b_ladder_p);
  tree->SetBranchAddress("module_t", module_p, &b_module_p);
  tree->SetBranchAddress("segment_t", segment_p, &b_segment_p);
  tree->SetBranchAddress("sstrips", &sstrips, &b_sstrips);
  tree->SetBranchAddress("HDSS_t", HDSS_t, &b_HDSS_t);
  // tree->SetBranchStatus("sstrips", false);
  // tree->SetBranchStatus("HDSS_t", false);

  doLoops();

}

void getModuleSizes::doLoops() {

  doLadderLoop();
  doModuleLoop();

}

void getModuleSizes::doLadderLoop(){

  ladders_per_layer.resize(LAYERS);
  for (unsigned int j = 0; j < LAYERS; j++) {
    ladders_per_layer[j] = -2;
  }

  for (int i = 0; i < events; i++ ) {
    b_ladder_p->GetEntry(i);
    for (unsigned int j = 0; j < LAYERS; j++) {
      if (ladder_p[j] > ladders_per_layer.at(j)) ladders_per_layer.at(j) = ladder_p[j];
    }
  }

  //cout << "ladders are: "<< endl;
  for (unsigned int j = 0; j < LAYERS; j++) {
    ladders_per_layer.at(j)++; // add 0
    //cout << j << " " << ladders_per_layer.at(j) << endl;
  }
}

void getModuleSizes::doModuleLoop() {
  modules_per_ladder_per_layer.resize(LAYERS);
  for (unsigned int j = 0; j < LAYERS; j++) {
    modules_per_ladder_per_layer.at(j).resize(ladders_per_layer.at(j));
    for (unsigned int k = 0; k < modules_per_ladder_per_layer.at(j).size(); k++) {
      modules_per_ladder_per_layer.at(j).at(k) = -2;
    }
  }

  for (int i = 0; i < events; i++ ) {
    b_ladder_p->GetEntry(i);
    b_module_p->GetEntry(i);
    for (unsigned int j = 0; j < LAYERS; j++) {
      for (int k = 0; k < ladders_per_layer.at(j); k++) {
        if (ladder_p[j] == k && module_p[j] > modules_per_ladder_per_layer.at(j).at(k)) modules_per_ladder_per_layer.at(j).at(k) = module_p[j];
      }
    }
  }

  //cout << "modules are: "<< endl;
  for (unsigned int j = 0; j < LAYERS; j++) {
    for (int k = 0; k < ladders_per_layer.at(j); k++) {
      modules_per_ladder_per_layer.at(j).at(k)++; // add 0
      //cout << j << " " << k << " " << modules_per_ladder_per_layer.at(j).at(k) << endl;
    }
  }

}

vector < vector < vector < int > > > getModuleSizes::getLayerLadderModule() {
  vector < vector < vector <int > > > subdetector_deg(LAYERS);
  for (unsigned int i = 0; i < subdetector_deg.size(); i++){
    subdetector_deg.at(i).resize(ladders_per_layer.at(i));
    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++){
      subdetector_deg.at(i).at(j).resize(modules_per_ladder_per_layer.at(i).at(j));
    }
  }

  //  cout << "modules are" << endl;
  for (unsigned int i = 0; i < subdetector_deg.size(); i++){
    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++){
        for (unsigned int k = 0; k < subdetector_deg.at(i).at(j).size(); k++){
          subdetector_deg.at(i).at(j).at(k) = 0;
      //    cout << i << " " << j << " " << subdetector_deg.at(i).at(j).size() << endl;
        }
    }
  }
  return subdetector_deg;
}

vector < vector < vector < int > > > getModuleSizes::getLayerLadderModuleOld() {

  vector < vector < vector <int > > > subdetector_deg(LAYERS);
  subdetector_deg.at(0).resize(3);
  subdetector_deg.at(1).resize(4);
  subdetector_deg.at(2).resize(6);
  subdetector_deg.at(3).resize(7);
  subdetector_deg.at(4).resize(10);
  subdetector_deg.at(5).resize(14);
  int cont = 0;
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) { // layer
    for (unsigned int j = 0; j < subdetector_deg.at(i).size(); j++) { // ladder

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

      for (unsigned int k = 0; k < subdetector_deg.at(i).at(j).size(); k++) { // module
        cont++;
      }
    }
  }
  return subdetector_deg;
}

int getModuleSizes::getStripsOn0() {
  if (stripsOn0 == -1) doStripsOn0Loop();
  return stripsOn0;
}

void getModuleSizes::doStripsOn0Loop() {
  int maxstrips = -1;
  for (int i = 0; i < events; i++ ) {
    b_sstrips->GetEntry(i);
    b_HDSS_t->GetEntry(i);
    for (int j = 0; j < sstrips[0]; j++) {
      if (HDSS_t[0][j] > maxstrips) {
        maxstrips = HDSS_t[0][j];
      }
    }
  }
  stripsOn0 = maxstrips;
}
