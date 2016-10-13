import ROOT
import math
import os,sys
import argparse

def CorrectNegativeBins(histo):
   hSS=histo.Clone()
   oldIntegral=hSS.Integral()
   if(oldIntegral>0):
      for k in range(0,hSS.GetXaxis().GetNbins()):
         if(hSS.GetBinContent(k+1)<0):
            oldContent=hSS.GetBinContent(k+1)
            diff=0.1-oldContent
            oldErr=hSS.GetBinError(k+1)
            newErr=math.sqrt(math.pow(oldErr,2)+math.pow(diff,2))
            hSS.SetBinContent(k+1,0.1)
            hSS.SetBinError(k+1,newErr)
      hSS.Scale(oldIntegral/hSS.Integral())
      return hSS
   else:
      return histo

def CheckZeroNormalization(hNominal,h):
   if(h.Integral()==0):
      oldName = h.GetName()
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
syst_mm     = ["","_JECUp","_JECDown","_TESUp","_TESDown","_MESUp","_MESDown","_bTagUp","_bTagDown","_RWeight1Up","_RWeight1Down","_RWeight2Up","_RWeight2Down"]
#syst_mm     = ["","_JECUp","_JECDown","_MESUp","_MESDown","_MRESUp","_MRESDown","_bTagUp","_bTagDown","_RWeight1Up","_RWeight1Down","_RWeight2Up","_RWeight2Down"]
dirs        = ["../data/ntuples/mt_analysis_new"]#,"../data/ntuples/mm_et_analysis_17June","../data/ntuples/mm_mt_analysis_new_17June","../data/ntuples/mm_tt_analysis_17June"]
channels    = ["mt"]#,"mm","mm","mm"]
channels2   = ["mt"]#em","et","mm","tt"]
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
       for s in range(0,len(syst_mm)):
          sys = syst_mm[s]
          hData = fData.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_data_obs"+sys+"_DataMG")
          hVV   = fVV.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_VV"+sys+"_DataMG")
          hTT   = fTT.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_TT"+sys+"_DataMG")
          hZLL  = fZLL.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_ZLL"+sys+"_DataMG")
          hZTT  = fZTT.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_ZTT"+sys+"_DataMG")
          hW    = fW.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_W"+sys+"_DataMG")
          hQCD  = fQCD.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_QCD"+sys+"_DataMG")
          hVBFH125  = fVBFH125.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_VBFH125"+sys+"_DataMG")
          hGGH125   = fGGH125.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_GGH125"+sys+"_DataMG")
          hEWKW = fEWKW.Get(channels[ich]+"_"+cat+"/h_low_mt_OS_EWKW"+sys+"_DataMG")

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
          
          hData.Rebin(2)#("data_obs"+sys)
          hVV.Rebin(2)#("VV"+sys)
          hTT.Rebin(2)#("TT"+sys)
          hZLL.Rebin(2)#("ZLL"+sys)
          hZTT.Rebin(2)#("ZTT"+sys)
          hW.Rebin(2)#("W"+sys)
          hQCD.Rebin(2)#("QCD"+sys)
          hVBFH125.Rebin(2)#("VBFH125"+sys)
          hGGH125.Rebin(2)#("GGH125"+sys)
          hEWKW.Rebin(2)#("EWKW"+sys)


          if(s==0):
             hDataNominal = CorrectNegativeBins(hData)
             hVVNominal   = CorrectNegativeBins(hVV)
             hTTNominal   = CorrectNegativeBins(hTT)
             hZLLNominal  = CorrectNegativeBins(hZLL)
             hZTTNominal  = CorrectNegativeBins(hZTT)
             hWNominal    = CorrectNegativeBins(hW)
             hQCDNominal  = CorrectNegativeBins(hQCD)
             hVBFNominal  = CorrectNegativeBins(hVBFH125)
             hGGNominal   = CorrectNegativeBins(hGGH125)
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
             hVBFCorrected   = CorrectNegativeBins(hVBFH125)
             hGGCorrected    = CorrectNegativeBins(hGGH125)
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
