#! /cvmfs/cms.cern.ch/slc6_amd64_gcc472/cms/cmssw/CMSSW_6_2_0_SLHC24/external/slc6_amd64_gcc472/bin/python

import os
import ROOT

i = 0
for filename in os.listdir("./plots"):
    if filename.endswith(".root") and filename.startswith("pat"):
        file = ROOT.TFile("./plots/" + filename)
        c = ROOT.TCanvas("c_" + str(i), "c_" + str(i), 600, 600)
        h_name = os.path.splitext(filename)[0]
        print h_name
        h = file.Get(h_name)
        h.Draw("COL")
        c.Print(r'plots/' + h_name + '.pdf')
        i = i + 1
