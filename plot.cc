#include "plot.h"

plot::plot(int x, int y) : tail(""),
                           dir("./") {
  c = new TCanvas("c", "c", x, y);
}

plot::~plot() {
}

void plot::plot1D(TH1D * h) {
  c->cd();
  h->Draw();
  c->Print(Form("%s/%s%s.pdf", dir.c_str(), h->GetName(), tail.c_str()));
  h->SaveAs(Form("%s/%s%s.root", dir.c_str(), h->GetName(), tail.c_str()));
}

void plot::plot2D(TH2D * h) {
  c->cd();
  h->SetStats(0);
  h->Draw("COL");
  c->Print(Form("%s/%s%s.pdf", dir.c_str(), h->GetName(), tail.c_str()));
  h->SaveAs(Form("%s/%s%s.root", dir.c_str(), h->GetName(), tail.c_str()));
}

void plot::add(TH1 *h1, Option_t* option) {

   if (!h1) return;
   if (!fHists) fHists = new TList();
   fHists->Add(h1,option);
}
