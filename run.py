#! /cvmfs/cms.cern.ch/slc6_amd64_gcc472/cms/cmssw/CMSSW_6_2_0_SLHC24/external/slc6_amd64_gcc472/bin/python

import ROOT
import os
import sys

file = ROOT.TFile("inputs/treeBank.root")
tree = file.Get("sector0")
entries = tree.GetEntries()
if len(sys.argv) == 2:
    jobs = int(sys.argv[1])
else:
    jobs = 20
print "The number of jobs is ", jobs

entries_per_job = int(entries/jobs)

ipattern = []
npattern = []
tail = []
log = []
for i in range(jobs):
    ipattern.append(i*entries_per_job)
    tail.append('_'+str(i))
    log.append(r'./logs/log_'+str(i)+'.out')

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
# option_m = "--makeTree"
option_m = " "

for i in range(jobs):
    command = [command0, command1, option_i,
               str(ipattern[i]), option_n, str(npattern[i]),
               option_t, tail[i], option_m, r'>', log[i], r'&']
    # print "command: ", command
    s = " ".join(command)
    print s
    os.system(s)
