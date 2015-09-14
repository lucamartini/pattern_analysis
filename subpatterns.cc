#include "options.h"
#include "plot.h"
#include "getModuleSizes.h"
#include "makesubbanks.h"

#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using namespace std;

void setLabels(TH1D * h, int type = 0) {

  if (type == 0) {
    h->GetXaxis()->SetBinLabel(1, "#phi_{1}Z_{1}");
    h->GetXaxis()->SetBinLabel(2, "#phi_{1}Z_{2}");
    h->GetXaxis()->SetBinLabel(3, "#phi_{2}Z_{1}");
    h->GetXaxis()->SetBinLabel(4, "#phi_{2}Z_{2}");
  }
  if (type == 1) {
    h->GetXaxis()->SetBinLabel(1, "#phi_{1}Z_{1}");
    h->GetXaxis()->SetBinLabel(2, "#phi_{1}Z_{2}");
    h->GetXaxis()->SetBinLabel(3, "#phi_{2}Z_{1}");
    h->GetXaxis()->SetBinLabel(4, "#phi_{2}Z_{2}");
  }
  if (type == 2) {
    h->GetXaxis()->SetBinLabel(1, "Z_{1}");
    h->GetXaxis()->SetBinLabel(2, "Z_{2}");
    h->GetXaxis()->SetBinLabel(3, "Z_{3}");
    h->GetXaxis()->SetBinLabel(4, "Z_{4}");
  }

  h->GetYaxis()->SetTitleOffset(1.2);
  h->SetMinimum(0);
  h->SetStats(0);
  h->SetLineWidth(2);


}

int main(int argc, char **argv) {

  options opt(argc, argv);
  int ievent = opt.get_ievent();
  int nevent = opt.get_nevent();
  string tail = opt.get_tail();
  string inputfile = opt.get_file();
  int type = opt.get_type();
  int SS5 = opt.get_SS5();

  vector <int> subbanks = {0, 0, 0, 0}; // phi1z1, phi1z2, phi2z1, phi2z2
  if (type == 1) subbanks.push_back(0); // 5th fake bank, that must be 0, check

  int count = 0;
  getModuleSizes * gms = new getModuleSizes(inputfile.c_str());
  gms->doLoops();
  vector < vector < vector < int > > > allModules = gms->getLayerLadderModule();
  vector < vector < vector < vector <int > > > > subdetector_deg;
  subdetector_deg.resize(allModules.size()); // layer
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) {
    if (type == 1 && i == 0) subdetector_deg.at(i).resize(allModules.at(i).size()-1); // ladder 0: 1 + 0.5
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
  delete gms;

  makesubbanks * sb = new makesubbanks(&subbanks, &subdetector_deg, inputfile.c_str(), nevent, ievent);
  sb->setSubbankType(type);
  if (type == 1) sb->setSS5(SS5);
  sb->doLoops();
  int patterns(sb->get_entries());
  cout << "patterns are " << patterns << endl;

  plot * plot_ = new plot();
  plot_->setTail(tail);
  plot_->setDir("./plots_temp/");

  int sum(0);
  for (int i = 0; i < 4; i++) {
    sum += subbanks[i];
  }

  ofstream subbankFile(Form("./subbanks/subbank%s.txt", tail.c_str()));
  cout << "in total, the bank is made by #patterns = " << patterns << " ( " << sum << " ) " << endl;
  cout << "the 4 subbanks are made by:" << endl;
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
        unsigned int subbanks_size =  subdetector_deg.at(i).at(j).at(k).size();
        if (type == 1) subbanks_size--; // real subbanks only
        for (unsigned int l = 0; l < subbanks_size; l++) { // for each real subbbank
          if (subdetector_deg.at(i).at(j).at(k).at(l) > 0) {
            deg++;
          }
        }
        if (deg == 0) cout << i << " " << j << " " << k << endl;
        subdetector_deg_h->Fill(deg);
        subdetector_deg_layer_h->Fill(deg);
        if (type == 1 && i == 0 && deg > 2) {
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
  if (type == 1) deg_p->plot1D(sb->get_SSsize_on_layer5());

  if (type == 1) {
    cout << "these fking deg in layer 0 (rightly) are: " << endl;
    for (unsigned i = 0; i < jk_v.size(); i++) {
      cout << jk_v.at(i).j << "  " << jk_v.at(i).k << endl;
    }
  }

  TH1D * subbanks_h = new TH1D("subbanks_h", Form("subbanks_h%s;subbanks;fraction of modules", tail.c_str()), 4, 0, 4);
  setLabels(subbanks_h, type);

  vector <int> ladder_module_integral(subdetector_deg.at(0).at(0).at(0).size(), 0);

  vector <TH1D *> subbanks_h_v;
  for (unsigned int i = 0; i < subdetector_deg.size(); i++) { // for each layer
    TH1D * subbanks_layer_h = new TH1D(Form("subbanks_layer_h_%d", i), Form("subbanks_layer_h_%d%s;subbanks;fraction of modules", i, tail.c_str()), 4, 0, 4);
    setLabels(subbanks_layer_h, type);
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
