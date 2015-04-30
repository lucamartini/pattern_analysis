#! /cvmfs/cms.cern.ch/slc6_amd64_gcc472/cms/cmssw/CMSSW_6_2_0_SLHC24/external/slc6_amd64_gcc472/bin/python

import os

os.system(r'hadd -f ./plots/pat_xy_h.root ./plots_temp/pat_xy_h_*.root')
os.system(r'hadd -f ./plots/pat_zr_h.root ./plots_temp/pat_zr_h_*.root')

os.system(r'mv ./plots_temp/sc_xy_h_0.root ./plots/sc_xy_h.root')
os.system(r'mv ./plots_temp/sc_xy_h_0.pdf ./plots/sc_xy_h.pdf')
os.system(r'mv ./plots_temp/sc_zr_h_0.root ./plots/sc_zr_h.root')
os.system(r'mv ./plots_temp/sc_zr_h_0.pdf ./plots/sc_zr_h.pdf')

os.system(r'mv ./plots_temp/pat_*_h_*.* ./temp/')
os.system(r'mv ./plots_temp/sc_*_h_*.* ./temp/')
