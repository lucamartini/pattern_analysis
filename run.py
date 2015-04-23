#! /cvmfs/cms.cern.ch/slc6_amd64_gcc472/cms/cmssw/CMSSW_6_2_0_SLHC24/external/slc6_amd64_gcc472/bin/python

import ROOT
import os

file = ROOT.TFile("treeBank.root")
tree = file.Get("sector0")
entries = tree.GetEntries()
jobs = 10
entries_per_job = int(entries/jobs)

ipattern = []
npattern = []
tail = []
log = []
for i in range(jobs):
    ipattern.append(i*entries_per_job)
    tail.append('_'+str(i))
    log.append('log_'+str(i)+'.out')

for i in range(jobs-1):
    npattern.append(ipattern[i+1] - ipattern[i])
npattern.append(-1)

print tail
print ipattern
print npattern

command0 = "nohup"
command1 = "./patterns"
option_i = "--ievent"
option_n = "--nevent"
option_t = "--tail"

for i in range(jobs):
    command = [command0, command1, option_i,
               str(ipattern[i]), option_n, str(npattern[i]),
               option_t, tail[i], r'>>', log[i], r'&']
    # print "command: ", command
    s = " ".join(command)
    print s
    os.system(s)
