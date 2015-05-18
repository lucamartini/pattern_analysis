#include "makesubbanks.h"

makesubbanks::makesubbanks(vector <int> * subbanks, vector < vector < vector < vector <int > > > > * subdetector_deg, string ttree_file, int nevent, int ievent) : patternloop(ttree_file, nevent, ievent), subdetector_deg_(subdetector_deg), subbanks_(subbanks), type_(0), SS5_(-1) {

}

makesubbanks::~makesubbanks() {
  if (SSsize_on_layer5) delete SSsize_on_layer5;
}

void makesubbanks::doLoops() {
  if (type_ == 0) do2phi6_2z();
  else if (type_ == 1) do2phi5_2z();
  else if (type_ == 2) do4z();
}

void makesubbanks::do2phi6_2z() {

for (int i = ievent_; i < max_entry; i++) {

    tree->GetEntry(i);
    int subbank = -1;
    if (i%100000==0) cout << i << " / " << max_entry << endl;
    if (ladder_p[1] < 2 && module_p[5] < 5) {
      subbanks_->at(0)++;
      subbank = 0;
    }
    if (ladder_p[1] < 2 && module_p[5] >= 5) {
      subbanks_->at(1)++;
      subbank = 1;
    }
    if (ladder_p[1] >= 2 && module_p[5] < 5) {
      subbanks_->at(2)++;
      subbank = 2;
    }
    if (ladder_p[1] >= 2 && module_p[5] >= 5) {
      subbanks_->at(3)++;
      subbank = 3;
    }

    for (int j = 0; j <= 5; j++) {
      subdetector_deg_->at(j).at(ladder_p[j]).at(module_p[j]).at(subbank)++; // in pattern i, subdetector j is in subbank

    }
  }
  cout << "pattern loop ended" << endl;

}

void makesubbanks::do2phi5_2z() {

  if (SS5_ == -1) {
    cout << "must setSS5!" << endl;
    exit(1);
  }

  SSsize_on_layer5 = new TH1D("SSsize_on_layer5", Form("SSsize_on_layer5 (#times %d)", SS5_), 10, 0., 10.);

  int strip0_MIN = 0;
  int strip0_MAX = (1024/SS5_) - 1;
  int strip0_MID = strip0_MAX / 2;

  for (int i = ievent_; i < max_entry; i++) {

    tree->GetEntry(i);
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
      subbanks_->at(4)++;
      subbank = 4;
      cout << min_strip << " " << max_strip << endl; // 0 by construction
    }
    else {
      if (first_half_in_ladders && module_p[5] < 5) {
        subbanks_->at(0)++;
        subbank = 0;
      }
      if (first_half_in_ladders && module_p[5] >= 5) {
        subbanks_->at(1)++;
        subbank = 1;
      }
      if (!first_half_in_ladders && module_p[5] < 5) {
        subbanks_->at(2)++;
        subbank = 2;
      }
      if (!first_half_in_ladders && module_p[5] >= 5) {
        subbanks_->at(3)++;
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

      subdetector_deg_->at(j).at(ladderino).at(module_p[j]).at(subbank)++; // subdetector is in subbank
    }
  }
  cout << "pattern loop ended" << endl;

}

void makesubbanks::do4z() {

for (int i = ievent_; i < max_entry; i++) {

    tree->GetEntry(i);
    int subbank = -1;
    if (i%100000==0) cout << i << " / " << max_entry << endl;
    if (module_p[5] < 3) {
      subbanks_->at(0)++;
      subbank = 0;
    }
    else if (module_p[5] < 5) {
      subbanks_->at(1)++;
      subbank = 1;
    }
    else if (module_p[5] < 7) {
      subbanks_->at(2)++;
      subbank = 2;
    }
    else if (module_p[5] < 10) {
      subbanks_->at(3)++;
      subbank = 3;
    }
    else {
      cout << "WTF module" << endl;
      exit(1);
    }

    for (int j = 0; j <= 5; j++) {
      subdetector_deg_->at(j).at(ladder_p[j]).at(module_p[j]).at(subbank)++; // in pattern i, subdetector j is in subbank

    }
  }
  cout << "pattern loop ended" << endl;

}
