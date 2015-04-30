#include "plot.h"

plot::plot(int x, int y) : tail("") {
  c = new TCanvas("c", "c", x, y);
}

plot::~plot() {
  delete c;
}

void plot::plot1D(TH1D * h) {
  h->Draw();
  c->Print(Form("%s/%s%s.pdf", dir.c_str(), h->GetName(), tail.c_str()));
  h->SaveAs(Form("%s/%s%s.root", dir.c_str(), h->GetName(), tail.c_str()));
}

void plot::plot2D(TH2D * h) {
  h->SetStats(0);
  h->Draw("COL");
  c->Print(Form("%s/%s%s.pdf", dir.c_str(), h->GetName(), tail.c_str()));
  h->SaveAs(Form("%s/%s%s.root", dir.c_str(), h->GetName(), tail.c_str()));
}