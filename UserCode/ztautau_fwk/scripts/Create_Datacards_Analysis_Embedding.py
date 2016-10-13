import ROOT
import math
import os,sys
import argparse


def CorrectNegativeBins(histo):
   hNew=histo.Clone()
   oldIntegral=hNew.Integral()
   if(oldIntegral>0): 
      for k in range(0,hNew.GetXaxis().GetNbins()):
         if(hNew.GetBinContent(k+1)<0):
            oldContent=hNew.GetBinContent(k+1)
            diff=0.1-oldContent
            oldErr=hNew.GetBinError(k+1)
            newErr=math.sqrt(math.pow(oldErr,2)+math.pow(diff,2))
            hNew.SetBinContent(k+1,0.1)
            hNew.SetBinError(k+1,newErr)
      hNew.Scale(oldIntegral/hNew.Integral())
      return hNew
   else:
      return histo


def CheckZeroNormalization(hNominal,h):
   if(h.Integral()==0):
      oldName = h.GetName()
      print "I am modidying the histogram"
      hCorrected = hNominal.Clone()
      hCorrected.Scale(0.1)
      for k in range(0,h.GetXaxis().GetNbins()):
         oldContent=hNominal.GetBinContent(k+1)
         oldErr=hNominal.GetBinError(k+1)
         newErr=math.sqrt(math.pow(oldErr,2)+math.pow(0.9*oldContent,2))
         hCorrected.SetBinError(k+1,newErr)
         hCorrected.SetName(oldName)
      return hCorrected
   else:
      return h

categories = ["inclusive","0jet", "1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","2bjet","MSSM_btag"]
syst_mm     = ["","_MESUp","_MESDown","_MRESUp","_MRESDown","_JECUp","_JECDown","_bTagUp","_bTagDown","_RWeight1Up","_RWeight1Down","_RWeight2Up","_RWeight2Down"]
embedding   = ["_DataMG_wTauSpinner"]

sys=""
i=-999

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
fQCD    = ROOT.TFile("../data/ntuples/tt_embedding//QCD.root" ,"r");

for ieb in embedding:
   fout= ROOT.TFile("ztt_tt_EB.inputs-sm-13TeV.root","recreate")
   for cat in categories:
       fout.cd()
       dir=fout.mkdir("mm_"+cat)
       dir.cd()

       for s in range(0,13):

          sys = syst_mm[s]+ieb

          hData = fData.Get("mm_"+cat+"/h_OS_data_obs"+sys)
          hVV   = fVV1.Get("mm_"+cat+"/h_OS_VV"+sys)
          hVV.Add(fVV2.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV3.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV4.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV5.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV6.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV7.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV8.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV9.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV10.Get("mm_"+cat+"/h_OS_VV"+sys),1)
          hVV.Add(fVV11.Get("mm_"+cat+"/h_OS_VV"+sys),1)

          hTT   = fTT.Get("mm_"+cat+"/h_OS_TT"+sys)
          
          hZLL  = fZLL1.Get("mm_"+cat+"/h_OS_ZLL"+sys)
          hZLL.Add(fZLL2.Get("mm_"+cat+"/h_OS_ZLL"+sys),1)
          hZLL.Add(fZLL3.Get("mm_"+cat+"/h_OS_ZLL"+sys),1)
          hZLL.Add(fZLL4.Get("mm_"+cat+"/h_OS_ZLL"+sys),1)
          hZLL.Add(fZLL5.Get("mm_"+cat+"/h_OS_ZLL"+sys),1)
          hZLL.Add(fZLL6.Get("mm_"+cat+"/h_OS_ZLL"+sys),1)
          hZLL.Add(fZLL7.Get("mm_"+cat+"/h_OS_ZLL"+sys),1)

          hZTT  = fZTT1.Get("mm_"+cat+"/h_OS_ZTT"+sys)
          hZTT.Add(fZTT2.Get("mm_"+cat+"/h_OS_ZTT"+sys),1)
          hZTT.Add(fZTT3.Get("mm_"+cat+"/h_OS_ZTT"+sys),1)
          hZTT.Add(fZTT4.Get("mm_"+cat+"/h_OS_ZTT"+sys),1)
          hZTT.Add(fZTT5.Get("mm_"+cat+"/h_OS_ZTT"+sys),1)
          hZTT.Add(fZTT6.Get("mm_"+cat+"/h_OS_ZTT"+sys),1)
          hZTT.Add(fZTT7.Get("mm_"+cat+"/h_OS_ZTT"+sys),1)

          hW    = fW1.Get("mm_"+cat+"/h_OS_W"+sys)
          hW.Add(fW2.Get("mm_"+cat+"/h_OS_W"+sys),1)
          hW.Add(fW3.Get("mm_"+cat+"/h_OS_W"+sys),1)
          hW.Add(fW4.Get("mm_"+cat+"/h_OS_W"+sys),1)
          hW.Add(fW5.Get("mm_"+cat+"/h_OS_W"+sys),1)

          hQCD  = fQCD.Get("mm_"+cat+"/h_OS_QCD"+sys)
          hVBF  = fVBF.Get("mm_"+cat+"/h_OS_VBFH125"+sys)
          hGG   = fGG.Get("mm_"+cat+"/h_OS_GGH125"+sys)
          hEWKW = fEWKW.Get("mm_"+cat+"/h_OS_EWKW"+sys)

          sys1 = syst_mm[s]

          hData.SetName("data_obs"+sys1)
          hVV.SetName("VV"+sys1)
          hTT.SetName("TT"+sys1)
          hZLL.SetName("ZLL"+sys1)
          hZTT.SetName("ZTT"+sys1)
          hW.SetName("W"+sys1)
          hQCD.SetName("QCD"+sys1)
          hVBF.SetName("VBFH125"+sys1)
          hGG.SetName("GGH125"+sys1)
          hEWKW.SetName("EWKW"+sys1)

          if(s==0):
             hDataNominal = CorrectNegativeBins(hData)  
             hVVNominal   = CorrectNegativeBins(hVV)   
             hTTNominal   = CorrectNegativeBins(hTT)   
             hZLLNominal  = CorrectNegativeBins(hZLL)   
             hZTTNominal  = CorrectNegativeBins(hZTT)   
             hWNominal    = CorrectNegativeBins(hW)   
             hQCDNominal  = CorrectNegativeBins(hQCD)   
             hVBFNominal  = CorrectNegativeBins(hVBF)   
             hGGNominal   = CorrectNegativeBins(hGG)   
             hEWKWNominal = CorrectNegativeBins(hEWKW)   
          
             hDataNominal.Write()
             hVVNominal.Write()
             hTTNominal.Write()
             hZLLNominal.Write()
             hZTTNominal.Write()
             hWNominal.Write()
             hQCDNominal.Write()
             hVBFNominal.Write()
             hGGNominal.Write()
             hEWKWNominal.Write()

          if(s!=0):
             hDataCorrected  = CorrectNegativeBins(hData)
             hVVCorrected    = CorrectNegativeBins(hVV)
             hTTCorrected    = CorrectNegativeBins(hTT)
             hZLLCorrected   = CorrectNegativeBins(hZLL)
             hZTTCorrected   = CorrectNegativeBins(hZTT)
             hWCorrected     = CorrectNegativeBins(hW)
             hQCDCorrected   = CorrectNegativeBins(hQCD)
             hVBFCorrected   = CorrectNegativeBins(hVBF)
             hGGCorrected    = CorrectNegativeBins(hGG)
             hEWKWCorrected  = CorrectNegativeBins(hEWKW)
             
             hDataFinal  = CheckZeroNormalization(hDataNominal,hDataCorrected)
             hVVFinal    = CheckZeroNormalization(hVVNominal,hVVCorrected)
             hTTFinal    = CheckZeroNormalization(hTTNominal,hTTCorrected)
             hZLLFinal   = CheckZeroNormalization(hZLLNominal,hZLLCorrected)
             hZTTFinal   = CheckZeroNormalization(hZTTNominal,hZTTCorrected)
             hWFinal     = CheckZeroNormalization(hWNominal,hWCorrected)
             hQCDFinal   = CheckZeroNormalization(hQCDNominal,hQCDCorrected)
             hVBFFinal   = CheckZeroNormalization(hVBFNominal,hVBFCorrected)
             hGGFinal    = CheckZeroNormalization(hGGNominal,hGGCorrected)
             hEWKWFinal  = CheckZeroNormalization(hEWKWNominal,hEWKWCorrected)

             hDataFinal.Write()
             hVVFinal.Write()
             hTTFinal.Write()
             hZLLFinal.Write()
             hZTTFinal.Write()
             hWFinal.Write()
             hQCDFinal.Write()
             hVBFFinal.Write()
             hGGFinal.Write()
             hEWKWFinal.Write()
