#!/usr/bin/env python
import os, re
import commands
import math, time
import sys, glob
import codecs, jinja2, stat, ntpath, sys, logging
from collections import OrderedDict as OD

print 
print 'START'
print 

samples = OD()

samples["DY1Jets"] = OD([
   ("labels", ["ZLL" ,"ZTT"]),
   ("nameout",["ZLL1","ZTT1"]),
   ("use_it", False),
   ("xsec",   1012.5), 
   ("nevents",65314144.0),
   ("npresel",9343479)
])
samples["DY2Jets"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL2","ZTT2"]),
   ("use_it", False),
   ("xsec",   332.8), 
   ("nevents",20019060),
   ("npresel",3076967)
])
samples["DY3Jets"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL3","ZTT3"]),
   ("use_it", False),
   ("xsec",   101.8),
   ("nevents",3961339),
   ("npresel",717949)
])
samples["DY4Jets"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL4","ZTT4"]),
   ("use_it", False),
   ("xsec",   54.8),
   ("nevents",4189017),
   ("npresel",686746)
])
samples["DYJets"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL5","ZTT5"]),
   ("use_it", True),
   ("xsec",   6025.2),
   ("nevents",232145248),
   ("npresel",29938192)
])
samples["EWKZToLL"] = OD([
   ("labels", ["ZLL", "ZTT"]),
   ("nameout",["ZLL6","ZTT6"]),
   ("use_it", False),
   ("xsec",   3.987),
   ("nevents",150000),
   ("npresel",38895)
])
samples["EWKWMinus"] = OD([
   ("labels", ["EWKW"]),
   ("nameout",["EWKWMinus"]),
   ("use_it", False),
   ("xsec",   20.25),
   ("nevents",490005),
   ("npresel",2750)
])
samples["EWKWPlus"] = OD([
   ("labels", ["EWKW"]),
   ("nameout",["EWKWPlus"]),
   ("use_it", False),
   ("xsec",   25.62),
   ("nevents",500000),
   ("npresel",2689)
])
samples["GluGluHToTauTau_M125"] = OD([
   ("labels", ["GGH125"]),
   ("nameout",["GGH125"]),
   ("use_it", False),
   ("xsec",   44.14*0.0627),
   ("nevents",1498800),
   ("npresel",15894)
])
samples["VBFHToTauTau_M125"] = OD([
   ("labels", ["VBFH125"]),
   ("nameout",["VBFH125"]),
   ("use_it", False),
   ("xsec",   3.782*0.0627),
   ("nevents",1497868),
   ("npresel",19691)
])
samples["ST_tW_antitop"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_tW_antitop"]),
   ("use_it", False),
   ("xsec",   35.6),
   ("nevents",999400),
   ("npresel",28472)
])
samples["ST_tW_top"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_tW_top"]),
   ("use_it", False),
   ("xsec",   35.6),
   ("nevents",1000000),
   ("npresel",28298)
])
samples["ST_t_antitop"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_t_antitop"]),
   ("use_it", False),
   ("xsec",   80.95),
   ("nevents",1613200),
   ("npresel",42616)
])
samples["ST_t_top"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ST_t_top"]),
   ("use_it", False),
   ("xsec",   136.02),
   ("nevents",3299200),
   ("npresel",82133)
])
samples["WW"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WW"]),
   ("use_it", False),
   ("xsec",   49.997),
   ("nevents",5056770),
   ("npresel",23712)
])
samples["WZJets"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZJets"]),
   ("use_it", False),
   ("xsec",   5.26),
   ("nevents",12258033),
   ("npresel",1316984)
])
samples["WZTo1L1Nu2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo1L1Nu2Q"]),
   ("use_it", False),
   ("xsec",   10.71),
   ("nevents",18674952),
   ("npresel",173738)
])
samples["WZTo1L3Nu"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo1L3Nu"]),
   ("use_it", False),
   ("xsec",   3.05),
   ("nevents",1672000),
   ("npresel",3262)
])
samples["WZTo2L2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["WZTo2L2Q"]),
   ("use_it", False),
   ("xsec",   5.595),
   ("nevents",24376544),
   ("npresel",2962135)
])
samples["ZZTo2L2Q"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ZZTo2L2Q"]),
   ("use_it", False),
   ("xsec",   3.22),
   ("nevents",15124709),
   ("npresel",2038154)
])
samples["ZZTo4L"] = OD([
   ("labels", ["VV"]),
   ("nameout",["ZZTo4L"]),
   ("use_it", False),
   ("xsec",   1.212),
   ("nevents",10059226),
   ("npresel",1563984)
])
samples["TT"] = OD([
   ("labels", ["TT"]),
   ("nameout",["TT"]),
   ("use_it", False),
   ("xsec",   831.76),
   ("nevents",279099168),
   ("npresel",9251229)
])
samples["W1Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W1Jets"]),
   ("use_it", False),
   ("xsec",   9644.5),
   ("nevents",45270196),
   ("npresel",40049)
])
samples["W2Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W2Jets"]),
   ("use_it", False),
   ("xsec",   3144.5),
   ("nevents",28043696),
   ("npresel",52607)
])
samples["W3Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W3Jets"]),
   ("use_it", False),
   ("xsec",   954.8),
   ("nevents",18307562),
   ("npresel",54143)
])
samples["W4Jets"] = OD([
   ("labels", ["W"]),
   ("nameout",["W4Jets"]),
   ("use_it", False),
   ("xsec",   485.6),
   ("nevents",8671800),
   ("npresel",42795)
])
samples["WJets"] = OD([
   ("labels", ["W"]),
   ("nameout",["WJets"]),
   ("use_it", False),
   ("xsec",   61526.7),
   ("nevents",47142232),
   ("npresel",22892)
])
samples["SingleMuon"] = OD([
   ("labels", ["data_obs"]),
   ("nameout",["data_obs"]),
   ("use_it", False),
   ("xsec",   1),
   ("nevents",72957712),
   ("npresel",2699287)
])


nsplit = 5000

for sample_name, sample_info in samples.items():
   nof_nameout = len(sample_info["nameout"])
   n_xsec   = sample_info["xsec"]
   n_events = sample_info["nevents"]
   n_presel = sample_info["npresel"]
   for i in range(nof_nameout):
   ##### creates directory and file list for job #######
      if not sample_info["use_it"]: continue
      njobs = int((n_presel / nsplit))+1
      dirScript = "tmp_EB_mt_analysis/"+sample_info["nameout"][i]+"/"
      fout = open("tmp_EB_mt_analysis/"+sample_info["nameout"][i]+"/resubmit.sh", 'w')
      count = 0
      for item in os.listdir(dirScript):
         fileName = item.split(".sh")
         if not "job" in fileName[0]: continue
         fileNumber = (fileName[0]).split("job")
         if not glob.glob("../data/ntuples/EB_mt_analysis/"+sample_info["nameout"][i]+"_"+fileNumber[1]+".root"):
            count=count+1 
            fout.write("bsub -q 1nd -o logs job"+fileNumber[1]+".sh \n")
         statinfo = os.stat("../data/ntuples/EB_mt_analysis/"+sample_info["nameout"][i]+"_"+fileNumber[1]+".root")
         fileSize = statinfo.st_size
         if(fileSize < 2000):
            fout.write("bsub -q 1nd -o logs job"+fileNumber[1]+".sh \n")

      if(count!=0):
         print "SAMPLE: %s ; NAMEOUT: %s; NJOBS: %s; STILL to Run: %s" %(sample_name,sample_info["nameout"][i],str(njobs),str(count))
      
print
print 'END'
print
