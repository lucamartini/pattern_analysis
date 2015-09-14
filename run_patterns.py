#! /cvmfs/cms.cern.ch/slc6_amd64_gcc472/cms/cmssw/CMSSW_6_2_0_SLHC24/external/slc6_amd64_gcc472/bin/python

import subprocess

# ./subpatterns_l5 -t _new_F -s 8 -f "inputs/treeBank_16_F.root "

command = ["./subpatterns"]
type = range(3)

tail_fountain = ["_sec24", "_sec24_F"]
SSs = [32, 8]
inputs = ["inputs/treeBank_3GeV_sec24.root",
          "inputs/treeBank_3GeV_sec24_F.root "]
# inputs = ["inputs/treeBank_sec16.root", "inputs/treeBank_sec16_F.root "]

n = 0
for i in range(3):
    for j in range(2):
        tail_ij = "_type" + str(type[i]) + tail_fountain[j]
        options_i = ["--tail", tail_ij,
                     "--type", str(type[i]),
                     "--SS5", str(SSs[j]),
                     "--file", inputs[j]]

        full_command = command + options_i
        print full_command
        subprocess.call(full_command)
        n = n+1

print "run", n, "times"
