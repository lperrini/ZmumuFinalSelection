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


categories = ["inclusive","0jet", "1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","2bjet","MSSM_btag"]
forQCD      = ["","_antiIso"]
syst_mm     = ["","_antiIso","_JECUp","_JECDown","_MESUp","_MESDown","_MRESUp","_MRESDown","_bTagUp","_bTagDown","_RWeight1Up","_RWeight1Down","_RWeight2Up","_RWeight2Down","_antiIso","_JECUp_antiIso","_JECDown_antiIso","_MESUp_antiIso","_MESDown_antiIso","_MRESUp_antiIso","_MRESDown_antiIso","_bTagUp_antiIso","_bTagDown_antiIso","_RWeight1Up_antiIso","_RWeight1Down_antiIso","_RWeight2Up_antiIso","_RWeight2Down_antiIso"]
nsyst       = len(syst_mm)
chcat       = ["OS","SS"]
dirs        = ["../data/ntuples/mm_analysis"]#,"../data/ntuples/mm_et_analysis_17June","../data/ntuples/mm_mt_analysis_17June","../data/ntuples/mm_tt_analysis_17June"] 
channels    = ["mm"]                                                                
histo       = [""]
weights     = ["","_DataMG_noMET_gen"]
ncr=2

nfiles = 1
fmm  = ROOT.TFile("../data/ntuples/mm_analysis/data_obs.root","r")
hScaling     = fmm.Get("mm_inclusive/h_OS_data_obs")
hScaling_pt  = fmm.Get("mm_inclusive/h_OS_pt_data_obs")
hScaling_eta = fmm.Get("mm_inclusive/h_OS_eta_data_obs")

for i in range(0,hScaling.GetXaxis().GetNbins()):
   hScaling.SetBinContent(i+1,1.10)
   hScaling.SetBinError(i+1,0.10)
for i in range(0,hScaling_pt.GetXaxis().GetNbins()):
   hScaling_pt.SetBinContent(i+1,1.10)
   hScaling_pt.SetBinError(i+1,0.10)
for i in range(0,hScaling_eta.GetXaxis().GetNbins()):
   hScaling_eta.SetBinContent(i+1,1.10)
   hScaling_eta.SetBinError(i+1,0.10)

for idir in range(0,nfiles):
    fData  = ROOT.TFile(dirs[idir]+"/data_obs.root","r")
    fVV    = ROOT.TFile(dirs[idir]+"/VV.root"  ,"r");
    fTT    = ROOT.TFile(dirs[idir]+"/TT.root"  ,"r");
    fZLL   = ROOT.TFile(dirs[idir]+"/ZLL.root" ,"r");
    fZTT   = ROOT.TFile(dirs[idir]+"/ZTT.root" ,"r");
    fW     = ROOT.TFile(dirs[idir]+"/W.root" ,"r");
    fVBF   = ROOT.TFile(dirs[idir]+"/VBFH125.root" ,"r");
    fGG    = ROOT.TFile(dirs[idir]+"/GGH125.root" ,"r");
    fEWKW  = ROOT.TFile(dirs[idir]+"/EWKW.root" ,"r");
    foutQCD= ROOT.TFile(dirs[idir]+"/QCD.root","recreate")

    for cat in categories:
       foutQCD.cd()
       dir=foutQCD.mkdir(channels[idir]+"_"+cat)
       dir.cd()
       for sys in range(0,nsyst/2):
          sys_shape   = ""
          sys_norma   = ""
          namehistoOS =""
          namehistoSS =""
          sys_shape   = syst_mm[sys+nsyst/2]
          sys_norma   = syst_mm[sys]
          namehistoOS = "h_OS"
          namehistoSS = "h_SS"

          for ihis in histo:
             for iw in weights:
                print channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_VV"+sys_shape+iw
                hOS_antiIso=fData.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_data_obs"+sys_shape+iw)
                hOS_antiIso.Add(fVV.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_VV"+sys_shape+iw),-1)
                hOS_antiIso.Add(fZTT.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_ZTT"+sys_shape+iw),-1)
                hOS_antiIso.Add(fZLL.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_ZLL"+sys_shape+iw),-1)
                hOS_antiIso.Add(fW.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_W"+sys_shape+iw),-1)
                hOS_antiIso.Add(fTT.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_TT"+sys_shape+iw),-1)
                hOS_antiIso.Add(fVBF.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_VBFH125"+sys_shape+iw),-1)
                hOS_antiIso.Add(fGG.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_GGH125"+sys_shape+iw),-1)
                hOS_antiIso.Add(fEWKW.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_EWKW"+sys_shape+iw),-1)

                hSS=fData.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_data_obs"+sys_norma+iw)
                hSS.Add(fVV.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_VV"+sys_norma+iw),-1)
                hSS.Add(fZTT.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_ZTT"+sys_norma+iw),-1)
                hSS.Add(fZLL.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_ZLL"+sys_norma+iw),-1)
                hSS.Add(fW.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_W"+sys_norma+iw),-1)
                hSS.Add(fTT.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_TT"+sys_norma+iw),-1)
                hSS.Add(fVBF.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_VBFH125"+sys_norma+iw),-1)
                hSS.Add(fGG.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_GGH125"+sys_norma+iw),-1)
                hSS.Add(fEWKW.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_EWKW"+sys_norma+iw),-1)

                hOS_antiIso.Scale(hSS.Integral()/hOS_antiIso.Integral())
                if(ihis==""): hScale = hScaling.Clone()
                if(ihis=="_pt"): hScale = hScaling_pt.Clone()
                if(ihis=="_eta"): hScale = hScaling_eta.Clone()
                hOS_antiIso.Multiply(hScale)
                hOS=CorrectQCD(hOS_antiIso)
                hOS.SetName(namehistoOS+ihis+"_QCD"+sys_norma+iw)
                hOS.Write()

    foutQCD.Close()


