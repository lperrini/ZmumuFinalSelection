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
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL0","ZTT0"]),
   ("xsec",   18610),
   ("nevents",62135700),
   ("npresel",509422),
   ("use_it", False)
])
samples["DY1Jets"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL1","ZTT1"]),
   ("xsec",   1012.5), 
   ("nevents",65314152),
   ("npresel",11827397),
   ("use_it", False)
])
samples["DY2Jets"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL2","ZTT2"]),
   ("xsec",   332.8), 
   ("nevents",20019058),
   ("npresel",4173330),
   ("use_it", False)
])
samples["DY3Jets"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL3","ZTT3"]),
   ("xsec",   101.8),
   ("nevents",5701878),
   ("npresel",1300325),
   ("use_it", False)
])
samples["DY4Jets"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL4","ZTT4"]),
   ("xsec",   54.8),
   ("nevents",4189017),
   ("npresel",1027184),
   ("use_it", False)
])
samples["DYJets_1"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_1","ZTT5_1"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9454920),
   ("npresel",1499876),#9454920),
   ("use_it", False)
])
samples["DYJets_2"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_2","ZTT5_2"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9042259),
   ("npresel",1432735),#9454920),
   ("use_it", False)
])
samples["DYJets_3"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_3","ZTT5_3"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9281360),
   ("npresel",1472987),
   ("use_it", False)
])
samples["DYJets_4"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_4","ZTT5_4"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9002947),
   ("npresel",1428816),
   ("use_it", False)
])
samples["DYJets_5"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_5","ZTT5_5"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#8943087),
   ("npresel",1421848),
   ("use_it", False)
])
samples["DYJets_6"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_6","ZTT5_6"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#8665958),
   ("npresel",1372872),
   ("use_it", False)
])
samples["DYJets_7"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_7","ZTT5_7"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9463085),
   ("npresel",1501379),
   ("use_it", False)
])
samples["DYJets_8"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_8","ZTT5_8"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9625145),
   ("npresel",1524261),
   ("use_it", False)
])
samples["DYJets_9"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_9","ZTT5_9"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9623887),
   ("npresel",1526584),
   ("use_it", False)
])
samples["DYJets_10"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_10","ZTT5_10"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9079679),
   ("npresel",1440595),
   ("use_it", False)
])
samples["DYJets_11"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_11","ZTT5_11"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9390718),
   ("npresel",1488651),
   ("use_it", False)
])
samples["DYJets_12"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_12","ZTT5_12"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9649272),
   ("npresel",1529639),
   ("use_it", False)
])
samples["DYJets_13"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_13","ZTT5_13"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9564274),
   ("npresel",1519347),
   ("use_it", False)
])
samples["DYJets_14"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_14","ZTT5_14"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9518684),
   ("npresel",1509123),
   ("use_it", False)
])
samples["DYJets_15"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_15","ZTT5_15"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9241418),
   ("npresel",1466234),
   ("use_it", False)
])
samples["DYJets_16"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_16","ZTT5_16"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9633077),
   ("npresel",1528990),
   ("use_it", False)
])
samples["DYJets_17"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_17","ZTT5_17"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9437116),
   ("npresel",1495432),
   ("use_it", False)
])
samples["DYJets_18"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_18","ZTT5_18"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9458298),
   ("npresel",1501376),
   ("use_it", False)
])
samples["DYJets_19"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_19","ZTT5_19"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9554103),
   ("npresel",1517446),
   ("use_it", False)
])
samples["DYJets_20"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_20","ZTT5_20"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9277188),
   ("npresel",1470521),
   ("use_it", False)
])
samples["DYJets_21"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_21","ZTT5_21"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9351484),
   ("npresel",1483685),
   ("use_it", False)
])
samples["DYJets_22"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_22","ZTT5_22"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9448680),
   ("npresel",1497074),
   ("use_it", False)
])
samples["DYJets_23"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_23","ZTT5_23"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9371599),
   ("npresel",1486494),
   ("use_it", False)
])
samples["DYJets_24"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_24","ZTT5_24"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#9193584),
   ("npresel",1457178),
   ("use_it", False)
])
samples["DYJets_25"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL5_25","ZTT5_25"]),
   ("xsec",   1012.5),
   ("nevents",232846987),#8557713),
   ("npresel",1356492),
   ("use_it", False)
])
samples["EWKZToLL"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL6","ZTT6"]),
   ("xsec",   3.987),
   ("nevents",150000),
   ("npresel",49059),
   ("use_it", False)
])
samples["EWKWMinus"] = OD([
   ("labels", ["EWKW"]),
   ("nameout",["EWKWMinus"]),
   ("xsec",   20.25),
   ("nevents",490005),
   ("npresel",83783),
   ("use_it", False)
])
samples["EWKWPlus"] = OD([
   ("labels", ["EWKW"]),
   ("nameout",["EWKWPlus"]),
   ("xsec",   25.62),
   ("nevents",500000),
   ("npresel",82752),
   ("use_it", False)
])
samples["GluGluHToTauTau_M125"] = OD([
   ("labels", ["GGH125"]),
   ("nameout",["GGH125"]),
   ("xsec",   44.14*0.0627),
   ("nevents",1498800),
   ("npresel",105676),
   ("use_it", False)
])
samples["VBFHToTauTau_M125"] = OD([
   ("labels", ["VBFH125"]),
   ("nameout",["VBFH125"]),
   ("xsec",   3.782*0.0627),
   ("nevents",1497868),
   ("npresel",146972),
   ("use_it", False)
])
samples["ST_tW_antitop"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_tW_antitop"]),
   ("xsec",   35.6),
   ("nevents",999400),
   ("npresel",137380),
   ("use_it", False)
])
samples["ST_tW_top"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_tW_top"]),
   ("xsec",   35.6),
   ("nevents",1000000),
   ("npresel",137390),
   ("use_it", False)
])
samples["ST_t_antitop"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_t_antitop"]),
   ("xsec",   80.95*3*0.108),
   ("nevents",1613200),
   ("npresel",270239),
   ("use_it", False)
])
samples["ST_t_top"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_t_top"]),
   ("xsec",   136.02*3*0.108),
   ("nevents",3299200),
   ("npresel",516430),
   ("use_it", False)
])
samples["WW"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WW"]),
   ("xsec",   49.997),
   ("nevents",5056770),
   ("npresel",686182),
   ("use_it", False)
])
samples["WZJets"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZJets"]),
   ("xsec",   5.26),
   ("nevents",12258033),
   ("npresel",2283242),
   ("use_it", False)
])
samples["WZTo1L1Nu2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo1L1Nu2Q"]),
   ("xsec",   10.71),
   ("nevents",18674952),
   ("npresel",2727004),
   ("use_it", False)
])
samples["WZTo1L3Nu"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo1L3Nu"]),
   ("xsec",   3.05),
   ("nevents",1672000),
   ("npresel",138731),
   ("use_it", False)
])
samples["WZTo2L2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo2L2Q"]),
   ("xsec",   5.595),
   ("nevents",24376540),
   ("npresel",4352132),
   ("use_it", False)
])
samples["ZZTo2L2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ZZTo2L2Q"]),
   ("xsec",   3.22),
   ("nevents",15124709),
   ("npresel",2908203),
   ("use_it", False)
])
samples["ZZTo4L"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ZZTo4L"]),
   ("xsec",   1.212),
   ("nevents",10059226),
   ("npresel",1998279),
   ("use_it", False)
])
samples["W1Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W1Jets"]),
   ("xsec",   9644.5),
   ("nevents",45270200),
   ("npresel",1717810),
   ("use_it", False)
])
samples["W2Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W2Jets"]),
   ("xsec",   3144.5),
   ("nevents",29512528),
   ("npresel",2285920),
   ("use_it", False)
])
samples["W3Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W3Jets"]),
   ("xsec",   954.8),
   ("nevents",18307562),
   ("npresel",1978651),
   ("use_it", False)
])
samples["W4Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W4Jets"]),
   ("xsec",   485.6),
   ("nevents",8671800),
   ("npresel",1195698),
   ("use_it", False)
])
samples["WJets"] = OD([
   ("labels", ["W"]),
   ("nameout",["WJets"]),
   ("xsec",   61526.7),
   ("nevents",47142244),
   ("npresel",758387),
   ("use_it", False)
])
samples["SingleMuon"] = OD([
   ("labels", ["data_obs"]),
   ("nameout",["data_obs"]),
   ("xsec",   1),
   ("nevents",72957712),
   ("npresel",9028774),
   ("use_it", True)
])
samples["TT_15"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_15"]),
   ("xsec",   831.76),
   ("nevents",279099156),#3880000),
   ("npresel",534325),
   ("use_it", False)
])
samples["TT_14"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_14"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19795108),
   ("npresel",2725597),
   ("use_it", False)
])
samples["TT_13"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_13"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19595268),
   ("npresel",2700652),
   ("use_it", False)
])
samples["TT_12"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_12"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19800000),
   ("npresel",2728943),
   ("use_it", False)
])
samples["TT_11"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_11"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19676924),
   ("npresel",2711639),
   ("use_it", False)
])
samples["TT_10"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_10"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19755024),
   ("npresel",2720686),
   ("use_it", False)
])
samples["TT_9"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_9"]),
   ("xsec",   831.76),
   ("nevents",279099156),#18860856),
   ("npresel",2611158),
   ("use_it", False)
])
samples["TT_8"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_8"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19681924),
   ("npresel",2711983),
   ("use_it", False)
])
samples["TT_7"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_7"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19537728),
   ("npresel",2736640),
   ("use_it", False)
])
samples["TT_6"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_6"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19415996),
   ("npresel",2702930),
   ("use_it", False)
])
samples["TT_5"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_5"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19252822),
   ("npresel",2651870),
   ("use_it", False)
])
samples["TT_4"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_4"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19715820),
   ("npresel",2715211),
   ("use_it", False)
])
samples["TT_3"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_3"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19835358),
   ("npresel",2735247),
   ("use_it", False)
])
samples["TT_2"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_2"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19855104),
   ("npresel",2733877),
   ("use_it", False)
])
samples["TT_1"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT_1"]),
   ("xsec",   831.76),
   ("nevents",279099156),#19803044),
   ("npresel",2727179),
   ("use_it", False)
])

outDir = "../data/ntuples/mt_embedding"
outDir2 = "mt_embedding"

path = os.getcwd()
print
print 'do not worry about folder creation:'

#os.system("rm -rf tmp_"+outDir2)
#os.system("rm -rf res_"+outDir2)
#os.system("rm -rf "+outDir)
#os.system("mkdir tmp_"+outDir2)
#os.system("mkdir res_"+outDir2)
#os.system("mkdir "+outDir)
print

nsplit = 5000

for sample_name, sample_info in samples.items():
   if not sample_info["use_it"]: continue
   nof_nameout = len(sample_info["nameout"])
   n_xsec   = sample_info["xsec"]
   n_events = sample_info["nevents"]
   n_presel = sample_info["npresel"]
   for i in range(nof_nameout):
   ##### creates directory and file list for job #######

      if(str(sample_info["nameout"][i]).find("ZLL")==0): continue;
      
      os.system("mkdir tmp_"+outDir2+"/"+str(sample_info["nameout"][i]))
      os.chdir("tmp_"+outDir2+"/"+str(sample_info["nameout"][i]))
      print "SAMPLE: %s ; NAMEOUT: %s" %(sample_name,sample_info["nameout"][i])
         
      nstart = 0
      nstop  = int(n_presel)
      njobs = int((n_presel / nsplit))+1
      if(nsplit<int(n_presel)):
         nstop=nsplit

      for ijobs in range(0,njobs):
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
         fout.write(ScriptName+ " inputFile=/store/user/veelken/Lucia/preselected_V2/"+sample_name+".root outputFile="+outDir+"/"+sample_info["nameout"][i]+"_"+str(ijobs)+".root xsection="+str(n_xsec)+" nevents="+str(n_events)+" sample="+sample_name+" label="+sample_info["labels"][i]+" channel=mm embedding=yes channelEB=mt ptcut1=19.0 etacut1=2.1 ptcut2=20.0 etacut2=2.3 met=yes dataOverMadgraphRW=no nstart="+str(nstart)+" nstop="+str(nstop)+" \n")
         fout.write("echo 'STOP---------------'\n")
         fout.write("echo\n")
         fout.write("echo\n")
         os.system("chmod 755 job"+str(ijobs)+".sh")
         os.system("bsub -q "+queue+" -o logs job"+str(ijobs)+".sh")
         nstart=nstart+nsplit
         nstop=nstop+nsplit
         if nstop>n_presel:
            nstop=n_presel
      os.chdir("../..")
   
   #os.chdir("../..")
   
print
#print "your jobs:"
#os.system("bjobs")
print
print 'END'
print
