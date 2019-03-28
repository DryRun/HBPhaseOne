Setup:

```
cmsrel CMSSW_10_6_0_pre2
cd CMSSW_10_6_0_pre2/src
cmsenv

# Pull unpacker changes
git cms-init
git cms-addpkg EventFilter/HcalRawToDigi
git remote add DryRun ssh://git@github.com/DryRun/cmssw
git fetch DryRun
git merge DryRun:ngHB

# Pull analysis skeleton
git clone ssh://git@github.com/DryRun/HBPhaseOne Analysis/HBPhaseOne

scram b -j8

cd Analysis/HBPhaseOne/analysis
cmsRun local_run.py run=328619 processEvents=1000
```