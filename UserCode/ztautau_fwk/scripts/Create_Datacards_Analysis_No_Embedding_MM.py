import ROOT
import math
import os,sys
import argparse


categories = ["inclusive","0jet", "1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","2bjet","MSSM_btag"]
syst_mm     = ["","_JECUp","_JECDown","_TESUp","_TESDown","_MESUp","_MESDown","_bTagUp","_bTagDown","_RWeight1Up","_RWeight1Down","_RWeight2Up","_RWeight2Down"]
#syst_mm     = ["","_JECUp","_JECDown","_MESUp","_MESDown","_MRESUp","_MRESDown","_bTagUp","_bTagDown","_RWeight1Up","_RWeight1Down","_RWeight2Up","_RWeight2Down"]
dirs        = ["../data/ntuples/mm_analysis"]#,"../data/ntuples/mm_et_analysis_17June","../data/ntuples/mm_mm_analysis_17June","../data/ntuples/mm_tt_analysis_17June"]
channels    = ["mm"]#,"mm","mm","mm"]
channels2   = ["mm"]#em","et","mm","tt"]
embedding   = [""]#,"_woTauSpinner","_wTauSpinner"]
ncr=2
#weights     = ["","_DataMG__noMET_gen","_DataMG__noMET_gen"]

sys=""
i=-999
nfiles = 1

for ich in range(0,nfiles):
   fData  = ROOT.TFile(dirs[ich]+"/data_obs.root","r")
   fVV    = ROOT.TFile(dirs[ich]+"/VV.root"  ,"r");
   fTT    = ROOT.TFile(dirs[ich]+"/TT.root"  ,"r");
   fZLL   = ROOT.TFile(dirs[ich]+"/ZLL.root" ,"r");
   fZTT   = ROOT.TFile(dirs[ich]+"/ZTT.root" ,"r");
   fW     = ROOT.TFile(dirs[ich]+"/W.root" ,"r");
   fVBFH125   = ROOT.TFile(dirs[ich]+"/VBFH125.root" ,"r");
   fGGH125    = ROOT.TFile(dirs[ich]+"/GGH125.root" ,"r");
   fQCD   = ROOT.TFile(dirs[ich]+"/QCD.root","r")
   fEWKW  = ROOT.TFile(dirs[ich]+"/EWKW.root","r")
   fout= ROOT.TFile("ztt_"+channels[ich]+".inputs-sm-13TeV.root","recreate")

   for cat in categories:
       fout.cd()
       dir=fout.mkdir(channels[ich]+"_"+cat)
       dir.cd()
       for s in range(0,1):
          sys = syst_mm[s]
          hData = fData.Get(channels[ich]+"_"+cat+"/h_OS_data_obs"+sys+"_DataMG_noMET_gen")
          hVV   = fVV.Get(channels[ich]+"_"+cat+"/h_OS_VV"+sys+"_DataMG_noMET_gen")
          hTT   = fTT.Get(channels[ich]+"_"+cat+"/h_OS_TT"+sys+"_DataMG_noMET_gen")
          hZLL  = fZLL.Get(channels[ich]+"_"+cat+"/h_OS_ZLL"+sys+"_DataMG_noMET_gen")
          hZTT  = fZTT.Get(channels[ich]+"_"+cat+"/h_OS_ZTT"+sys+"_DataMG_noMET_gen")
          hW    = fW.Get(channels[ich]+"_"+cat+"/h_OS_W"+sys+"_DataMG_noMET_gen")
          hQCD  = fQCD.Get(channels[ich]+"_"+cat+"/h_OS_QCD"+sys+"_DataMG_noMET_gen")
          hVBFH125  = fVBFH125.Get(channels[ich]+"_"+cat+"/h_OS_VBFH125"+sys+"_DataMG_noMET_gen")
          hGGH125   = fGGH125.Get(channels[ich]+"_"+cat+"/h_OS_GGH125"+sys+"_DataMG_noMET_gen")
          hEWKW = fEWKW.Get(channels[ich]+"_"+cat+"/h_OS_EWKW"+sys+"_DataMG_noMET_gen")

          hData.SetName("data_obs"+sys)
          hVV.SetName("VV"+sys)
          hTT.SetName("TT"+sys)
          hZLL.SetName("ZLL"+sys)
          hZTT.SetName("ZTT"+sys)
          hW.SetName("W"+sys)
          hQCD.SetName("QCD"+sys)
          hVBFH125.SetName("VBFH125"+sys)
          hGGH125.SetName("GGH125"+sys)
          hEWKW.SetName("EWKW"+sys)
    
          hData.Write()
          hVV.Write()
          hTT.Write()
          hZLL.Write()
          hZTT.Write()
          hW.Write()
          hQCD.Write()
          hVBFH125.Write()
          hGGH125.Write()
          hEWKW.Write()
