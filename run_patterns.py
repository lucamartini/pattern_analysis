#! /cvmfs/cms.cern.ch/slc6_amd64_gcc472/cms/cmssw/CMSSW_6_2_0_SLHC24/external/slc6_amd64_gcc472/bin/python

import subprocess

# ./subpatterns_l5 -t _new_F -s 8 -f "inputs/treeBank_16_F.root "

command = "./subpatterns"
tail_l5 = ["", "_l5"]

tail_fountain = ["", "_F"]
SSs = [32, 8]
inputs = ["inputs/treeBank_16.root", "inputs/treeBank_16_F.root "]

for i in range(2):
    for j in range(2):
        command_i = [command + tail_l5[i]]
        tail = tail_l5[i] + tail_fountain[j]
        options_i = ["--tail", tail, "--SS5", str(SSs[j]), "--file", inputs[j]]

        full_command = command_i + options_i
        print full_command
        subprocess.call(full_command)
