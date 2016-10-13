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


categories  = ["inclusive","0jet", "1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","MSSM_btag","1jet","2jets"]
categories2 = ["inclusive","0jet", "1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","2bjet","MSSM_btag","1jet","2jets"]
syst_mt     = ["","_JECUp","_JECDown","_TESUp","_TESDown","_MESUp","_MESDown","_bTagUp","_bTagDown","_RWeight1Up","_RWeight1Down","_RWeight2Up","_RWeight2Down","_antiIso","_JECUp_antiIso","_JECDown_antiIso","_TESUp_antiIso","_TESDown_antiIso","_MESUp_antiIso","_MESDown_antiIso","_bTagUp_antiIso","_bTagDown_antiIso","_RWeight1Up_antiIso","_RWeight1Down_antiIso","_RWeight2Up_antiIso","_RWeight2Down_antiIso"]
nsyst       = len(syst_mt)
mtcat       = ["low_mt","high_mt"]
chcat       = ["OS","SS"]
mtdir       = ["../data/ntuples/mt_analysis_new"]
dirs        = ["../data/ntuples/mt_analysis_new"]
weights     = ["","_DataMG"]
histo       = [""]
channels    = ["mt"]
ncr=2

fmt  = ROOT.TFile("../data/ntuples/mt_analysis_new/data_obs.root","r")
hScaling_mass_mt  = fmt.Get("mt_inclusive/h_low_mt_OS_data_obs")
hScaling_pt_mt    = fmt.Get("mt_inclusive/h_low_mt_OS_pt_data_obs")
hScaling_eta_mt   = fmt.Get("mt_inclusive/h_low_mt_OS_eta_data_obs")
for i in range(0,hScaling_mass_mt.GetXaxis().GetNbins()):
   hScaling_mass_mt.SetBinContent(i+1,1.10)
   hScaling_mass_mt.SetBinError(i+1,0.10)
for i in range(0,hScaling_pt_mt.GetXaxis().GetNbins()):
   hScaling_pt_mt.SetBinContent(i+1,1.10)
   hScaling_pt_mt.SetBinError(i+1,0.10)
for i in range(0,hScaling_eta_mt.GetXaxis().GetNbins()):
   hScaling_eta_mt.SetBinContent(i+1,1.10)
   hScaling_eta_mt.SetBinError(i+1,0.10)

for idir in mtdir:
   fData  = ROOT.TFile(idir+"/data_obs.root","r")
   fVV    = ROOT.TFile(idir+"/VV.root"  ,"r");
   fTT    = ROOT.TFile(idir+"/TT.root"  ,"r");
   fZLL   = ROOT.TFile(idir+"/ZLL.root" ,"r");
   fZTT   = ROOT.TFile(idir+"/ZTT.root" ,"r");
   fW     = ROOT.TFile(idir+"/W.root" ,"r");
   fVBF   = ROOT.TFile(idir+"/VBFH125.root" ,"r");
   fGG    = ROOT.TFile(idir+"/GGH125.root" ,"r");
   fEWKW  = ROOT.TFile(idir+"/EWKW.root" ,"r");

   foutW= ROOT.TFile(idir+"/W_DD.root","recreate")
   for cat in categories:
      foutW.cd()
      dir=foutW.mkdir("mt_"+cat)
      dir.cd()
      for sys in syst_mt:
         for iw in weights:
            for ihis in histo:
               for i in range(0,ncr):
                  hWSig = fW.Get("mt_"+cat+"/h_"+mtcat[0]+"_"+chcat[i]+ihis+"_W"+sys+iw)
                  hWSid = fW.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_W"+sys+iw)
                  hWSig_1 = hWSig.Clone()
                  hWSid_1 = hWSid.Clone()
                  hWSig_1.Rebin(hWSig.GetXaxis().GetNbins())
                  hWSid_1.Rebin(hWSid.GetXaxis().GetNbins())
                  hSid = fData.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_data_obs"+sys+iw)
                  hSid.Add(fVV.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_VV"+sys+iw),-1)
                  hSid.Add(fZTT.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_ZTT"+sys+iw),-1)
                  hSid.Add(fZLL.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_ZLL"+sys+iw),-1)
                  hSid.Add(fTT.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_TT"+sys+iw),-1)
                  hSid.Add(fVBF.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_VBFH125"+sys+iw),-1) 
                  hSid.Add(fGG.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_GGH125"+sys+iw),-1)   
                  hSid.Add(fEWKW.Get("mt_"+cat+"/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_EWKW"+sys+iw),-1)   
                  hSidC=CorrectQCD(hSid)
                  hSid_1=hSidC.Clone() 
                  hSid_1.Rebin(hSidC.GetXaxis().GetNbins()) #normalization extraction from data in the sideband region
                  hRatioW = hWSig_1.Clone()
                  hRatioW.Divide(hWSid_1) #ratio from W signal/sideband
                  hScale = hSid_1.Clone()
                  hScale.Multiply(hRatioW) #multiply data * scale factor from W
                  if(hWSig.Integral()>0):
                     hWSig.Scale(hScale.GetBinContent(1)/hWSig.Integral())
                  hWSig.SetName("h_"+mtcat[0]+"_"+chcat[i]+ihis+"_W"+sys+iw)
                  hWSig.Write()
   dir=foutW.mkdir("mt_2bjet")
   dir.cd()
   for sys in syst_mt:
      for iw in weights:
         for ihis in histo:
            for i in range(0,ncr):
               hWSig_loose   = fW.Get("mt_2bjet_loose/h_"+mtcat[0]+"_"+chcat[i]+ihis+"_W"+sys+iw) #from here just the shape
               hWSig         = fW.Get("mt_2bjet/h_"+mtcat[0]+"_"+chcat[i]+ihis+"_W"+sys+iw) #from here the normalization
               hWSid         = fW.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_W"+sys+iw)
               if(hWSig_loose.Integral()>0):
                  hWSig_loose.Scale(hWSig.Integral()/hWSig_loose.Integral())
               hWSig_1       = hWSig.Clone()
               hWSid_1       = hWSid.Clone()
               hWSig_1.Rebin(hWSig.GetXaxis().GetNbins())
               hWSid_1.Rebin(hWSid.GetXaxis().GetNbins())

               hSid = fData.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_data_obs"+sys+iw)
               hSid.Add(fVV.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_VV"+sys+iw),-1)
               hSid.Add(fZTT.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_ZTT"+sys+iw),-1)
               hSid.Add(fZLL.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_ZLL"+sys+iw),-1)
               hSid.Add(fTT.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_TT"+sys+iw),-1)
               hSid.Add(fVBF.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_VBFH125"+sys+iw),-1) 
               hSid.Add(fGG.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_GGH125"+sys+iw),-1)   
               hSid.Add(fEWKW.Get("mt_2bjet/h_"+mtcat[1]+"_"+chcat[i]+ihis+"_EWKW"+sys+iw),-1)   
               hSidC=CorrectQCD(hSid)
               hSid_1=hSidC.Clone() 
               hSid_1.Rebin(hSid.GetXaxis().GetNbins()) #normalization extraction from data in the sideband region
               hRatioW = hWSig_1.Clone()
               hRatioW.Divide(hWSid_1) #ratio from W signal/sideband
               hScale = hSid_1.Clone()
               hScale.Multiply(hRatioW) #multiply data * scale factor from W
               if(hWSig_loose.Integral()>0):
                  hWSig_loose.Scale(hScale.GetBinContent(1)/hWSig_loose.Integral())
               hWSig.SetName("h_"+mtcat[0]+"_"+chcat[i]+ihis+"_W"+sys+iw)
               hWSig.Write()
   foutW.Close()

   os.system("mv "+idir+"/W.root "+idir+"/W_MC.root")
   os.system("mv "+idir+"/W_DD.root "+idir+"/W.root")

for idir in range(0,1):
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

   for cat in categories2:
      foutQCD.cd()
      dir=foutQCD.mkdir(channels[idir]+"_"+cat)
      dir.cd()
      for s in range(0,nsyst/2):
         sys_shape   = ""
         sys_norma   = ""
         namehistoOS = ""
         namehistoSS = ""
         hScaling = ROOT.TH1F()
         sys_shape = syst_mt[s+nsyst/2]
         sys_norma = syst_mt[s]
         namehistoOS = "h_low_mt_OS" 
         namehistoSS = "h_low_mt_SS" 

         for ihis in histo:
            for iw in weights:
               #for QCD shape
               hOS_antiIso=fData.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_data_obs"+sys_shape+iw)
               hOS_antiIso.Add(fVV.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_VV"+sys_shape+iw),-1)
               hOS_antiIso.Add(fZTT.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_ZTT"+sys_shape+iw),-1)
               hOS_antiIso.Add(fZLL.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_ZLL"+sys_shape+iw),-1)
               hOS_antiIso.Add(fW.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_W"+sys_shape+iw),-1)
               hOS_antiIso.Add(fTT.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_TT"+sys_shape+iw),-1)
               hOS_antiIso.Add(fVBF.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_VBFH125"+sys_shape+iw),-1)
               hOS_antiIso.Add(fGG.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_GGH125"+sys_shape+iw),-1)
               hOS_antiIso.Add(fEWKW.Get(channels[idir]+"_"+cat+"/"+namehistoOS+ihis+"_EWKW"+sys_shape+iw),-1)
            
               #for QCD normalization
               hSS=fData.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_data_obs"+sys_norma+iw)
               hSS.Add(fVV.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_VV"+sys_norma+iw),-1)
               hSS.Add(fZTT.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_ZTT"+sys_norma+iw),-1)
               hSS.Add(fZLL.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_ZLL"+sys_norma+iw),-1)
               hSS.Add(fW.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_W"+sys_norma+iw),-1)
               hSS.Add(fTT.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_TT"+sys_norma+iw),-1)
               hSS.Add(fVBF.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_VBFH125"+sys_norma+iw),-1)
               hSS.Add(fGG.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_GGH125"+sys_norma+iw),-1)
               hSS.Add(fEWKW.Get(channels[idir]+"_"+cat+"/"+namehistoSS+ihis+"_EWKW"+sys_norma+iw),-1)

               if(ihis==""): hScale = hScaling_mass_mt.Clone()
               if(ihis=="_pt"): hScale = hScaling_pt_mt.Clone()
               if(ihis=="_eta"): hScale = hScaling_eta_mt.Clone()
               #normalize shape to the correct normalization
               hOS_antiIso.Scale(hSS.Integral()/hOS_antiIso.Integral())
               hOS_antiIso.Multiply(hScale)
               hOS=CorrectQCD(hOS_antiIso)
               hOS.SetName(namehistoOS+ihis+"_QCD"+sys_norma+iw)
               hOS.Write()

   foutQCD.Close()

