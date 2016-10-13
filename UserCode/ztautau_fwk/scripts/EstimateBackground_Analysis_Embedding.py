import ROOT
import math
import os,sys
import argparse

def CorrectQCD(histo):
   hSS=histo.Clone()
   oldIntegral=hSS.Integral()
   for k in range(0,hSS.GetXaxis().GetNbins()):
      if(hSS.GetBinContent(k+1)<0):
         oldContent=hSS.GetBinContent(k+1)
         diff=0.1-oldContent
         oldErr=hSS.GetBinError(k+1)
         newErr=math.sqrt(math.pow(oldErr,2)+math.pow(diff,2))
         hSS.SetBinContent(k+1,0.1)
         hSS.SetBinError(k+1,newErr)
   if(oldIntegral>0):
      hSS.Scale(oldIntegral/hSS.Integral())
   return hSS


categories  = ["inclusive","0jet", "1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","MSSM_btag"]
categories2 = ["inclusive","0jet", "1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","2bjet","MSSM_btag"]
syst_mm     = ["","_MRESUp","_MRESDown","_MESUp","_MESDown","_JECUp","_JECDown","_bTagUp","_bTagDown","_RWeight1Up","_RWeight1Down","_RWeight2Up","_RWeight2Down","_antiIso","_MRESUp_antiIso","_MRESDown_antiIso","_MESUp_antiIso","_MESDown_antiIso","_JECUp_antiIso","_JECDown_antiIso","_bTagUp_antiIso","_bTagDown_antiIso","_RWeight1Up_antiIso","_RWeight1Down_antiIso","_RWeight2Up_antiIso","_RWeight2Down_antiIso"]
chcat       = ["OS","SS"]
channels    = ["mm"]      
embedding   = ["_DataMG"]#_wTauSpinner"]
ncr=2
eos_path  = "/store/user/veelken/Lucia/tt_embedding/"

os.system("cmsStage -f "+eos_path+"/data_obs.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ST_tW_antitop.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ST_tW_top.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ST_t_antitop.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ST_t_top.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/WW.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/WZJets.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/WZTo1L1Nu2Q.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/WZTo1L3Nu.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/WZTo2L2Q.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZZTo2L2Q.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZZTo4L.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZLL0.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZLL1.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZLL2.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZLL3.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZLL4.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZLL5.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZLL6.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZTT0.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZTT1.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZTT2.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZTT3.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZTT4.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZTT5.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/ZTT6.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/TT.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/W1Jets.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/W2Jets.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/W3Jets.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/W4Jets.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/WJets.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/VBFH125.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/GGH125.root ../data/ntuples/tt_embedding/");
os.system("cmsStage -f "+eos_path+"/EWKW.root ../data/ntuples/tt_embedding/");

#TFile::Open(("root://eoscms.cern.ch//eos/cms//"
fmm  = ROOT.TFile("../data/ntuples/tt_embedding/data_obs.root","r")
hScaling_mm    = fmm.Get("mm_inclusive/h_OS_data_obs")
for i in range(0,hScaling_mm.GetXaxis().GetNbins()):
   hScaling_mm.SetBinContent(i+1,1.10)
   hScaling_mm.SetBinError(i+1,0.10)
for idir in range(0,1):
   fData   = ROOT.TFile("../data/ntuples/tt_embedding//data_obs.root","r")

   fVV1    = ROOT.TFile("../data/ntuples/tt_embedding//ST_tW_antitop.root"  ,"r");
   fVV2    = ROOT.TFile("../data/ntuples/tt_embedding//ST_tW_top.root"  ,"r");
   fVV3    = ROOT.TFile("../data/ntuples/tt_embedding//ST_t_antitop.root"  ,"r");
   fVV4    = ROOT.TFile("../data/ntuples/tt_embedding//ST_t_top.root"  ,"r");
   fVV5    = ROOT.TFile("../data/ntuples/tt_embedding//WW.root"  ,"r");
   fVV6    = ROOT.TFile("../data/ntuples/tt_embedding//WZJets.root"  ,"r");
   fVV7    = ROOT.TFile("../data/ntuples/tt_embedding//WZTo1L1Nu2Q.root"  ,"r");
   fVV8    = ROOT.TFile("../data/ntuples/tt_embedding//WZTo1L3Nu.root"  ,"r");
   fVV9    = ROOT.TFile("../data/ntuples/tt_embedding//WZTo2L2Q.root"  ,"r");
   fVV10   = ROOT.TFile("../data/ntuples/tt_embedding//ZZTo2L2Q.root"  ,"r");
   fVV11   = ROOT.TFile("../data/ntuples/tt_embedding//ZZTo4L.root"  ,"r");

   fZLL1   = ROOT.TFile("../data/ntuples/tt_embedding//ZLL0.root" ,"r");
   fZLL2   = ROOT.TFile("../data/ntuples/tt_embedding//ZLL1.root" ,"r");
   fZLL3   = ROOT.TFile("../data/ntuples/tt_embedding//ZLL2.root" ,"r");
   fZLL4   = ROOT.TFile("../data/ntuples/tt_embedding//ZLL3.root" ,"r");
   fZLL5   = ROOT.TFile("../data/ntuples/tt_embedding//ZLL4.root" ,"r");
   fZLL6   = ROOT.TFile("../data/ntuples/tt_embedding//ZLL5.root" ,"r");
   fZLL7   = ROOT.TFile("../data/ntuples/tt_embedding//ZLL6.root" ,"r");
   
   fZTT1   = ROOT.TFile("../data/ntuples/tt_embedding//ZTT0.root" ,"r");
   fZTT2   = ROOT.TFile("../data/ntuples/tt_embedding//ZTT1.root" ,"r");
   fZTT3   = ROOT.TFile("../data/ntuples/tt_embedding//ZTT2.root" ,"r");
   fZTT4   = ROOT.TFile("../data/ntuples/tt_embedding//ZTT3.root" ,"r");
   fZTT5   = ROOT.TFile("../data/ntuples/tt_embedding//ZTT4.root" ,"r");
   fZTT6   = ROOT.TFile("../data/ntuples/tt_embedding//ZTT5.root" ,"r");
   fZTT7   = ROOT.TFile("../data/ntuples/tt_embedding//ZTT6.root" ,"r");
   
   fTT     = ROOT.TFile("../data/ntuples/tt_embedding//TT.root"  ,"r");
   
   fW1     = ROOT.TFile("../data/ntuples/tt_embedding//W1Jets.root" ,"r");
   fW2     = ROOT.TFile("../data/ntuples/tt_embedding//W2Jets.root" ,"r");
   fW3     = ROOT.TFile("../data/ntuples/tt_embedding//W3Jets.root" ,"r");
   fW4     = ROOT.TFile("../data/ntuples/tt_embedding//W4Jets.root" ,"r");
   fW5     = ROOT.TFile("../data/ntuples/tt_embedding//WJets.root" ,"r");
   
   fVBF    = ROOT.TFile("../data/ntuples/tt_embedding//VBFH125.root" ,"r");
   fGG     = ROOT.TFile("../data/ntuples/tt_embedding//GGH125.root" ,"r");
   fEWKW   = ROOT.TFile("../data/ntuples/tt_embedding//EWKW.root" ,"r");
   foutQCD = ROOT.TFile("../data/ntuples/tt_embedding/QCD.root","recreate")
   
   for cat in categories2:
      foutQCD.cd()
      dir=foutQCD.mkdir(channels[idir]+"_"+cat)
      dir.cd()
      for s in range(0,13):
         hScaling = ROOT.TH1F()
         hScaling = hScaling_mm.Clone()
         for iemb in embedding:
            sys_shape   = ""
            sys_norma   = ""
            namehistoOS = ""
            namehistoSS = ""
            sys_shape   = syst_mm[s]+"_antiIso"+iemb
            sys_norma   = syst_mm[s]+iemb
            namehistoOS = "h_OS" 
            namehistoSS = "h_SS" 

            #for QCD shape
            hOS_antiIso=fData.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_data_obs"+sys_shape)
            print channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape         
            hOS_antiIso.Add(fVV1.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV2.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV3.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV4.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV5.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV6.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV7.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV8.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV9.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV10.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)
            hOS_antiIso.Add(fVV11.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VV"+sys_shape),-1)

            hOS_antiIso.Add(fZTT1.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZTT"+sys_shape),-1)
            hOS_antiIso.Add(fZTT2.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZTT"+sys_shape),-1)
            hOS_antiIso.Add(fZTT3.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZTT"+sys_shape),-1)
            hOS_antiIso.Add(fZTT4.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZTT"+sys_shape),-1)
            hOS_antiIso.Add(fZTT5.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZTT"+sys_shape),-1)
            hOS_antiIso.Add(fZTT6.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZTT"+sys_shape),-1)
            hOS_antiIso.Add(fZTT7.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZTT"+sys_shape),-1)

            hOS_antiIso.Add(fZLL1.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZLL"+sys_shape),-1)
            hOS_antiIso.Add(fZLL2.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZLL"+sys_shape),-1)
            hOS_antiIso.Add(fZLL3.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZLL"+sys_shape),-1)
            hOS_antiIso.Add(fZLL4.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZLL"+sys_shape),-1)
            hOS_antiIso.Add(fZLL5.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZLL"+sys_shape),-1)
            hOS_antiIso.Add(fZLL6.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZLL"+sys_shape),-1)
            hOS_antiIso.Add(fZLL7.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_ZLL"+sys_shape),-1)

            hOS_antiIso.Add(fW1.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_W"+sys_shape),-1)
            hOS_antiIso.Add(fW2.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_W"+sys_shape),-1)
            hOS_antiIso.Add(fW3.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_W"+sys_shape),-1)
            hOS_antiIso.Add(fW4.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_W"+sys_shape),-1)
            hOS_antiIso.Add(fW5.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_W"+sys_shape),-1)

            hOS_antiIso.Add(fTT.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_TT"+sys_shape),-1)
            hOS_antiIso.Add(fVBF.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_VBFH125"+sys_shape),-1)
            hOS_antiIso.Add(fGG.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_GGH125"+sys_shape),-1)
            hOS_antiIso.Add(fEWKW.Get(channels[idir]+"_"+cat+"/"+namehistoOS+"_EWKW"+sys_shape),-1)
            
            #for QCD normalization
            """
            print "####### STARTING "+channels[idir]+"_"+cat+ " --> systematic "+sys_norma
            print "Data WO EMB "+ str((fData.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_data_obs"+syst_mm[s])).Integral())+" W EMB "+str((fData.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_data_obs"+sys_norma)).Integral())
            print "VV1 WO EMB "+ str((fVV1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV2 WO EMB "+ str((fVV2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV3 WO EMB "+ str((fVV3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV4 WO EMB "+ str((fVV4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV5 WO EMB "+ str((fVV5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV6 WO EMB "+ str((fVV6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV7 WO EMB "+ str((fVV7.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV7.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV8 WO EMB "+ str((fVV8.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV8.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV9 WO EMB "+ str((fVV9.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV9.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV10 WO EMB "+ str((fVV10.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV10.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "VV11 WO EMB "+ str((fVV11.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+syst_mm[s])).Integral())+" W EMB "+str((fVV11.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma)).Integral())
            print "ZTT1 WO EMB "+ str((fZTT1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+syst_mm[s])).Integral())+" W EMB "+str((fZTT1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma)).Integral())
            print "ZTT2 WO EMB "+ str((fZTT2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+syst_mm[s])).Integral())+" W EMB "+str((fZTT2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma)).Integral())
            print "ZTT3 WO EMB "+ str((fZTT3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+syst_mm[s])).Integral())+" W EMB "+str((fZTT3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma)).Integral())
            print "ZTT4 WO EMB "+ str((fZTT4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+syst_mm[s])).Integral())+" W EMB "+str((fZTT4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma)).Integral())
            print "ZTT5 WO EMB "+ str((fZTT5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+syst_mm[s])).Integral())+" W EMB "+str((fZTT5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma)).Integral())
            print "ZTT6 WO EMB "+ str((fZTT6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+syst_mm[s])).Integral())+" W EMB "+str((fZTT6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma)).Integral())
            print "ZLL1 WO EMB "+ str((fZLL1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+syst_mm[s])).Integral())+" W EMB "+str((fZLL1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma)).Integral())
            print "ZLL2 WO EMB "+ str((fZLL2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+syst_mm[s])).Integral())+" W EMB "+str((fZLL2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma)).Integral())
            print "ZLL3 WO EMB "+ str((fZLL3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+syst_mm[s])).Integral())+" W EMB "+str((fZLL3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma)).Integral())
            print "ZLL4 WO EMB "+ str((fZLL4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+syst_mm[s])).Integral())+" W EMB "+str((fZLL4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma)).Integral())
            print "ZLL5 WO EMB "+ str((fZLL5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+syst_mm[s])).Integral())+" W EMB "+str((fZLL5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma)).Integral())
            print "ZLL6 WO EMB "+ str((fZLL6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+syst_mm[s])).Integral())+" W EMB "+str((fZLL6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma)).Integral())
            print "W1 WO EMB "+ str((fW1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+syst_mm[s])).Integral())+" W EMB "+str((fW1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma)).Integral())
            print "W2 WO EMB "+ str((fW2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+syst_mm[s])).Integral())+" W EMB "+str((fW2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma)).Integral())
            print "W3 WO EMB "+ str((fW3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+syst_mm[s])).Integral())+" W EMB "+str((fW3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma)).Integral())
            print "W4 WO EMB "+ str((fW4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+syst_mm[s])).Integral())+" W EMB "+str((fW4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma)).Integral())
            print "W5 WO EMB "+ str((fW5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+syst_mm[s])).Integral())+" W EMB "+str((fW5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma)).Integral())
            print "TT WO EMB "+ str((fTT.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_TT"+syst_mm[s])).Integral())+" W EMB "+str((fTT.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_TT"+sys_norma)).Integral())
            print "VBFH125 WO EMB "+ str((fVBF.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VBFH125"+syst_mm[s])).Integral())+" W EMB "+str((fVBF.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VBFH125"+sys_norma)).Integral())
            print "GGH125 WO EMB "+ str((fGG.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_GGH125"+syst_mm[s])).Integral())+" W EMB "+str((fGG.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_GGH125"+sys_norma)).Integral())
            print "EWKW WO EMB "+ str((fEWKW.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_EWKW"+syst_mm[s])).Integral())+" W EMB "+str((fEWKW.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_EWKW"+sys_norma)).Integral())
            """

            hSS=fData.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_data_obs"+sys_norma)
            hSS.Add(fVV1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV7.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV8.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV9.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV10.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fVV11.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VV"+sys_norma),-1)
            hSS.Add(fZTT1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma),-1)
            hSS.Add(fZTT2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma),-1)
            hSS.Add(fZTT3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma),-1)
            hSS.Add(fZTT4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma),-1)
            hSS.Add(fZTT5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma),-1)
            hSS.Add(fZTT6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma),-1)
            hSS.Add(fZTT7.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZTT"+sys_norma),-1)
            hSS.Add(fZLL1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma),-1)
            hSS.Add(fZLL2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma),-1)
            hSS.Add(fZLL3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma),-1)
            hSS.Add(fZLL4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma),-1)
            hSS.Add(fZLL5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma),-1)
            hSS.Add(fZLL6.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma),-1)
            hSS.Add(fZLL7.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_ZLL"+sys_norma),-1)
            hSS.Add(fW1.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma),-1)
            hSS.Add(fW2.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma),-1)
            hSS.Add(fW3.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma),-1)
            hSS.Add(fW4.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma),-1)
            hSS.Add(fW5.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_W"+sys_norma),-1)
            hSS.Add(fTT.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_TT"+sys_norma),-1)
            hSS.Add(fVBF.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_VBFH125"+sys_norma),-1)
            hSS.Add(fGG.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_GGH125"+sys_norma),-1)
            hSS.Add(fEWKW.Get(channels[idir]+"_"+cat+"/"+namehistoSS+"_EWKW"+sys_norma),-1)

            #normalize shape to the correct normalization

            hOS_antiIso.Scale(hSS.Integral()/hOS_antiIso.Integral())
            hOS_antiIso.Multiply(hScaling*0.2)
            hOS=CorrectQCD(hOS_antiIso)
            hOS.SetName(namehistoOS+"_QCD"+sys_norma+"_wTauSpinner")
            hOS.Write()

   foutQCD.Close()

