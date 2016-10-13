#!/usr/bin/env python
import os, re
import commands
import math, time
import sys
import codecs, jinja2, stat, ntpath, sys, logging
from collections import OrderedDict as OD




print 
print 'START'
print 
########   YOU ONLY NEED TO FILL THE AREA BELOW   #########
########   customization  area #########
ScriptName = "runZtautauAnalysis" # script to be used with cmsRun
queue = "1nw" # give bsub queue -- 8nm (8 minutes), 1nh (1 hour), 8nh, 1nd (1day), 2nd, 1nw (1 week), 2nw 
########   customization end   #########

samples = OD()
samples["DYJetsLowMass"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL0","ZTT0"]),
   ("xsec",   18610),
   ("nevents",62080652),
   ("use_it", True)
])
samples["DY1Jets"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL1","ZTT1"]),
   ("xsec",   1012.5), 
   ("nevents",65254912),
   ("use_it", True)
])
samples["DY2Jets"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL2","ZTT2"]),
   ("xsec",   332.8), 
   ("nevents",20019058),
   ("use_it", True)
])
samples["DY3Jets"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL3","ZTT3"]),
   ("xsec",   101.8),
   ("nevents",5701878),
   ("use_it", True)
])
samples["DY4Jets"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL4","ZTT4"]),
   ("xsec",   54.8),
   ("nevents",4189017),
   ("use_it", True)
])
samples["DYJets_1"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_1","ZTT5_1"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9454920),
   ("use_it", True)
])
samples["DYJets_2"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_2","ZTT5_2"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9042259),
   ("use_it", True)
])
samples["DYJets_3"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_3","ZTT5_3"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9281360),
   ("use_it", True)
])
samples["DYJets_4"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_4","ZTT5_4"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9002947),
   ("use_it", True)
])
samples["DYJets_5"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_5","ZTT5_5"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#8943087),
   ("use_it", True)
])
samples["DYJets_6"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_6","ZTT5_6"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#8665958),
   ("use_it", True)
])
samples["DYJets_7"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_7","ZTT5_7"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9463085),
   ("use_it", True)
])
samples["DYJets_8"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_8","ZTT5_8"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9625145),
   ("use_it", True)
])
samples["DYJets_9"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_9","ZTT5_9"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9623887),
   ("use_it", True)
])
samples["DYJets_10"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_10","ZTT5_10"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9079679),
   ("use_it", True)
])
samples["DYJets_11"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_11","ZTT5_11"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9390718),
   ("use_it", True)
])
samples["DYJets_12"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_12","ZTT5_12"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9649272),
   ("use_it", True)
])
samples["DYJets_13"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_13","ZTT5_13"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9564274),
   ("use_it", True)
])
samples["DYJets_14"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_14","ZTT5_14"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9518684),
   ("use_it", True)
])
samples["DYJets_15"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_15","ZTT5_15"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9241418),
   ("use_it", True)
])
samples["DYJets_16"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_16","ZTT5_16"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9633077),
   ("use_it", True)
])
samples["DYJets_17"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_17","ZTT5_17"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9437116),
   ("use_it", True)
])
samples["DYJets_18"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_18","ZTT5_18"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9458298),
   ("use_it", True)
])
samples["DYJets_19"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_19","ZTT5_19"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9554103),
   ("use_it", True)
])
samples["DYJets_20"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_20","ZTT5_20"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9277188),
   ("use_it", True)
])
samples["DYJets_21"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_21","ZTT5_21"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9351484),
   ("use_it", True)
])
samples["DYJets_22"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_22","ZTT5_22"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9448680),
   ("use_it", True)
])
samples["DYJets_23"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_23","ZTT5_23"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9371599),
   ("use_it", True)
])
samples["DYJets_24"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_24","ZTT5_24"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9193584),
   ("use_it", True)
])
samples["DYJets_25"] = OD([
   ("labels", ["ZLL","ZTT"]),
   ("nameout",["ZLL5_25","ZTT5_25"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#8557713),
   ("use_it", True)
])
samples["EWKZToLL"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL6","ZTT6"]),
   ("xsec",   3.987),
   ("nevents",150000),
   ("use_it", True)
])
samples["EWKWMinus"] = OD([
   ("labels", ["EWKW"]),
   ("nameout",["EWKWMinus"]),
   ("xsec",   20.25),
   ("nevents",490005),
   ("use_it", True)
])
samples["EWKWPlus"] = OD([
   ("labels", ["EWKW"]),
   ("nameout",["EWKWPlus"]),
   ("xsec",   25.62),
   ("nevents",500000),
   ("use_it", True)
])
samples["GluGluHToTauTau_M125"] = OD([
   ("labels", ["GGH125"]),
   ("nameout",["GGH125"]),
   ("xsec",   44.14*0.0627),
   ("nevents",1498800),
   ("use_it", True)
])
samples["VBFHToTauTau_M125"] = OD([
   ("labels", ["VBFH125"]),
   ("nameout",["VBFH125"]),
   ("xsec",   3.782*0.0627),
   ("nevents",1497868),
   ("use_it", True)
])
samples["ST_tW_antitop"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_tW_antitop"]),
   ("xsec",   35.6),
   ("nevents",999400),
   ("use_it", True)
])
samples["ST_tW_top"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_tW_top"]),
   ("xsec",   35.6),
   ("nevents",1000000),
   ("use_it", True)
])
samples["ST_t_antitop"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_t_antitop"]),
   ("xsec",   80.95*3*0.108),
   ("nevents",1613200),
   ("use_it", True)
])
samples["ST_t_top"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_t_top"]),
   ("xsec",   136.02*3*0.108),
   ("nevents",3299200),
   ("use_it", True)
])
samples["WW"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WW"]),
   ("xsec",   49.997),
   ("nevents",5056770),
   ("use_it", True)
])
samples["WZJets"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZJets"]),
   ("xsec",   5.26),
   ("nevents",12258033),
   ("use_it", True)
])
samples["WZTo1L1Nu2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo1L1Nu2Q"]),
   ("xsec",   10.71),
   ("nevents",18674952),
   ("use_it", True)
])
samples["WZTo1L3Nu"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo1L3Nu"]),
   ("xsec",   3.05),
   ("nevents",1672000),
   ("use_it", True)
])
samples["WZTo2L2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo2L2Q"]),
   ("xsec",   5.595),
   ("nevents",24376540),
   ("use_it", True)
])
samples["ZZTo2L2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ZZTo2L2Q"]),
   ("xsec",   3.22),
   ("nevents",15124709),
   ("use_it", True)
])
samples["ZZTo4L"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ZZTo4L"]),
   ("xsec",   1.212),
   ("nevents",10059226),
   ("use_it", True)
])
samples["W1Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W1Jets"]),
   ("xsec",   9644.5),
   ("nevents",45270200),
   ("use_it", True)
])
samples["W2Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W2Jets"]),
   ("xsec",   3144.5),
   ("nevents",29512528),
   ("use_it", True)
])
samples["W3Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W3Jets"]),
   ("xsec",   954.8),
   ("nevents",18302492),
   ("use_it", True)
])
samples["W4Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W4Jets"]),
   ("xsec",   485.6),
   ("nevents",8671800),
   ("use_it", True)
])
samples["WJets"] = OD([
   ("labels", ["W"]),
   ("nameout",["WJets"]),
   ("xsec",   61526.7),
   ("nevents",47142244),
   ("use_it", True)
])
samples["SingleMuon"] = OD([
   ("labels", ["data_obs"]),
   ("nameout",["data_obs"]),
   ("xsec",   1),
   ("nevents",72957712),
   ("use_it", True)
])
samples["TT_15"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_15"]),
   ("xsec",   831.76),
   ("nevents",279099156),#3880000),
   ("use_it", True)
])
samples["TT_14"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_14"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19795108),
   ("use_it", True)
])
samples["TT_13"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_13"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19595268),
   ("use_it", True)
])
samples["TT_12"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_12"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19800000),
   ("use_it", True)
])
samples["TT_11"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_11"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19676924),
   ("use_it", True)
])
samples["TT_10"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_10"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19755024),
   ("use_it", True)
])
samples["TT_9"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_9"]),
   ("xsec",   831.76),
   ("nevents",279099156),#18860856),
   ("use_it", True)
])
samples["TT_8"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_8"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19681924),
   ("use_it", True)
])
samples["TT_7"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_7"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19537728),
   ("use_it", True)
])
samples["TT_6"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_6"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19415996),
   ("use_it", True)
])
samples["TT_5"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_5"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19252822),
   ("use_it", True)
])
samples["TT_4"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_4"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19715820),
   ("use_it", True)
])
samples["TT_1"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_1"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19803044),
   ("use_it", True)
])
samples["TT_2"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_2"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19855104),
   ("use_it", True)
])
samples["TT_3"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_3"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19835358),
   ("use_it", True)
])

###NB: here you give the name to the output dir
###check it carefully all the time before running!!!
outDir = "../data/ntuples/mt_analysis_new"
outDir2 = "mt_analysis_new"

path = os.getcwd()
print
print 'do not worry about folder creation:'

os.system("rm -rf tmp_"+outDir2)
os.system("rm -rf res_"+outDir2)
os.system("rm -rf "+outDir)
os.system("mkdir tmp_"+outDir2)
os.system("mkdir res_"+outDir2)
os.system("mkdir "+outDir)
print

for sample_name, sample_info in samples.items():
   if not sample_info["use_it"]: continue
   nof_nameout = len(sample_info["nameout"])
   n_xsec   = sample_info["xsec"]
   n_events = sample_info["nevents"]
   for i in range(nof_nameout):
   ##### creates directory and file list for job #######
      #print "SAMPLE: %s ; NAMEOUT: %s" %(sample_name,sample_info["nameout"][i])
      os.system("mkdir tmp_"+outDir2+"/"+str(sample_info["nameout"][i]))
      os.chdir("tmp_"+outDir2+"/"+str(sample_info["nameout"][i]))
      
      for ijobs in range(0,1):
         fout = open('job'+str(ijobs)+'.sh', 'w')
         fout.write("#!/bin/sh\n")
         fout.write("echo\n")
         fout.write("echo\n")
         fout.write("echo 'START---------------'\n")
         fout.write("echo 'WORKDIR ' ${PWD}\n")
         fout.write("source /afs/cern.ch/cms/cmsset_default.sh\n")
         fout.write("cd "+str(path)+"/../\n")
         fout.write("cmsenv\n")
         fout.write("cd "+str(path)+"\n")
         ######## TO RUN MUTAU selection
         fout.write(ScriptName+ " inputFile=/store/user/veelken/Lucia/preselected_V4/"+sample_name+".root outputFile="+outDir+"/"+sample_info["nameout"][i]+".root xsection="+str(n_xsec)+" nevents="+str(n_events)+" sample="+sample_name+" label="+sample_info["labels"][i]+" channel=mt embedding=no channelEB=mt ptcut1=19.0 etacut1=2.1 ptcut2=20.0 etacut2=2.3 met=yes dataOverMadgraphRW=no \n")
         ######## TO RUN MUMU selection
         ######## the flag channelEB=mt basically can contain any information, as it is not used when the embedding is not running!!!
         #fout.write(ScriptName+ " inputFile=/store/user/veelken/Lucia/preselected_V4/"+sample_name+".root outputFile="+outDir+"/"+sample_info["nameout"][i]+".root xsection="+str(n_xsec)+" nevents="+str(n_events)+" sample="+sample_name+" label="+sample_info["labels"][i]+" channel=mm embedding=no channelEB=mt ptcut1=19.0 etacut1=2.1 ptcut2=20.0 etacut2=2.3 met=yes dataOverMadgraphRW=no \n")
         fout.write("echo 'STOP---------------'\n")
         fout.write("echo\n")
         fout.write("echo\n")
         os.system("chmod 755 job"+str(ijobs)+".sh")
         os.system("bsub -q "+queue+" -o logs job"+str(ijobs)+".sh")
      os.chdir("../..")
   
   #os.chdir("../..")
   
print
#print "your jobs:"
#os.system("bjobs")
print
print 'END'
print
