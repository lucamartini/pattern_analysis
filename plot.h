#ifndef PLOT_H
#define PLOT_H

#include <string>
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"

class plot {
 public:
  plot(int x = 600, int y = 600);
  ~plot();

  void plot1D(TH1D * h);
  void plot2D(TH2D * h);

  void setTail(std::string newTail) {tail = newTail;};
  void setDir(std::string newDir) {dir = newDir;};

 private:
  TCanvas * c;
  std::string tail;
  std::string dir;

};


#endif
