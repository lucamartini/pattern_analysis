#! /cvmfs/cms.cern.ch/slc6_amd64_gcc472/cms/cmssw/CMSSW_6_2_0_SLHC24/external/slc6_amd64_gcc472/bin/python

import os

subbanks = [0, 0, 0, 0]
for filename in os.listdir("./subbanks"):
    file = open("./subbanks/" + filename, "r")
    subbank_i = file.readlines()
    for i in range(4):
        subbanks[i] += int(subbank_i[i])

print subbanks
