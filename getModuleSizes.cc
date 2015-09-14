#include "getModuleSizes.h"

getModuleSizes::getModuleSizes(string ttree_file, int nevent, int ievent) : patternloop(ttree_file, nevent, ievent), stripsOn0(-1) {

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

  for (int i = ievent_; i < max_entry; i++ ) {
    b_ladder_p->GetEntry(i);
    for (unsigned int j = 0; j < LAYERS; j++) {
      if (ladder_p[j] > ladders_per_layer.at(j)) ladders_per_layer.at(j) = ladder_p[j];
    }
  }

  cout << "ladders are: "<< endl;
  for (unsigned int j = 0; j < LAYERS; j++) {
    ladders_per_layer.at(j)++; // add 0
    cout << j << " " << ladders_per_layer.at(j) << endl;
  }
  //cout << "ladder loop done" << endl;
}

void getModuleSizes::doModuleLoop() {
  modules_per_ladder_per_layer.resize(LAYERS);
  for (unsigned int j = 0; j < LAYERS; j++) {
    modules_per_ladder_per_layer.at(j).resize(ladders_per_layer.at(j));
    for (unsigned int k = 0; k < modules_per_ladder_per_layer.at(j).size(); k++) {
      modules_per_ladder_per_layer.at(j).at(k) = -2;
    }
  }

  for (int i = ievent_; i < max_entry; i++ ) {
    b_ladder_p->GetEntry(i);
    b_module_p->GetEntry(i);
    for (unsigned int j = 0; j < LAYERS; j++) {
      for (int k = 0; k < ladders_per_layer.at(j); k++) {
        if (ladder_p[j] == k && module_p[j] > modules_per_ladder_per_layer.at(j).at(k)) modules_per_ladder_per_layer.at(j).at(k) = module_p[j];
      }
    }
  }

  cout << "modules are: "<< endl;
  for (unsigned int j = 0; j < LAYERS; j++) {
    for (int k = 0; k < ladders_per_layer.at(j); k++) {
      modules_per_ladder_per_layer.at(j).at(k)++; // add 0
      cout << j << " " << k << " " << modules_per_ladder_per_layer.at(j).at(k) << endl;
    }
  }

  if (sector==17) {
    cout << "making fixes for sector " << sector << " ..."  << endl;
    modules_per_ladder_per_layer.at(4).at(0) = 9;
    modules_per_ladder_per_layer.at(5).at(13) = 9;
  }

  cout << "module loop done" << endl;

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
  for (int i = ievent_; i < max_entry; i++ ) {
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
