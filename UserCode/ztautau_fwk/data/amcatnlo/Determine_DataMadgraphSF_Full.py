if __name__ == "__main__":

    import ROOT
    import math


    def Correct(histo):
       h=histo.Clone()
       oldIntegral = h.Integral()
       for ix in range(0,h.GetXaxis().GetNbins()+1):
          for iy in range(0,h.GetYaxis().GetNbins()+1):
             for iz in range(0,h.GetZaxis().GetNbins()+1):
                if(h.GetBinContent(ix,iy,iz)<0):
                   oldContent = h.GetBinContent(ix,iy,iz)
                   diff =0.1-oldContent
                   oldErr=h.GetBinError(ix,iy,iz)
                   newErr=math.sqrt(math.pow(oldErr,2)+math.pow(diff,2))
                   h.SetBinContent(ix,iy,iz,0.1)
                   h.SetBinError(ix,iy,iz,newErr)
       if(oldIntegral>0):
          h.Scale(oldIntegral/h.Integral())
       return h 


    ntupleDir = "../ntuples/mm_weights/"
    his       = ["rweight_3d_noMET","rweight_3d_EM_noMET","rweight_3d_ET_noMET","rweight_3d_MT_noMET","rweight_3d_TT_noMET",
                 "rweight_3d_MET","rweight_3d_EM_MET","rweight_3d_ET_MET","rweight_3d_MT_MET","rweight_3d_TT_MET",
                 "rweight_3d_noMET_fixed_bins","rweight_3d_EM_noMET_fixed_bins","rweight_3d_ET_noMET_fixed_bins","rweight_3d_MT_noMET_fixed_bins","rweight_3d_TT_noMET_fixed_bins",
                 "rweight_3d_MET_fixed_bins","rweight_3d_EM_MET_fixed_bins","rweight_3d_ET_MET_fixed_bins","rweight_3d_MT_MET_fixed_bins","rweight_3d_TT_MET_fixed_bins"]

    nhis = len(his)
    fZTT=ROOT.TFile(ntupleDir+"ZTT.root","r")
    fZLL=ROOT.TFile(ntupleDir+"ZLL.root","r")
    fW=ROOT.TFile(ntupleDir+"W.root","r")
    fTT=ROOT.TFile(ntupleDir+"TT.root","r")
    fVV=ROOT.TFile(ntupleDir+"VV.root","r")
    fGGH125=ROOT.TFile(ntupleDir+"GGH125.root","r")
    fVBFH125=ROOT.TFile(ntupleDir+"VBFH125.root","r")
    fEWKW=ROOT.TFile(ntupleDir+"EWKW.root","r")
    fData=ROOT.TFile(ntupleDir+"data_obs.root","r")
    
    fout=ROOT.TFile("reweighting_NB.root","recreate")

    print nhis
    for i in range(0,nhis):

       hDataOS   = fData.Get(his[i]+"_data_obs")
       hTotBkgOS = fVV.Get(his[i]+"_VV")
       hTotBkgOS.Add(fZTT.Get(his[i]+"_ZTT"),1)
       hTotBkgOS.Add(fW.Get(his[i]+"_W"),1)
       hTotBkgOS.Add(fTT.Get(his[i]+"_TT"),1)
       hTotBkgOS.Add(fVBFH125.Get(his[i]+"_VBFH125"),1)
       hTotBkgOS.Add(fGGH125.Get(his[i]+"_GGH125"),1)
       hTotBkgOS.Add(fEWKW.Get(his[i]+"_EWKW"),1)

       hTotBkg = hTotBkgOS.Clone()
       hData   = hDataOS.Clone()

       hDataOS.Add(hTotBkgOS,-1)
       hSigOS  = fZLL.Get(his[i]+"_ZLL")
   
       hDataOS.Scale(1./hDataOS.Integral())
       hSigOS.Scale(1./hSigOS.Integral())
       
       ratioOS=hDataOS.Clone()
       ratioOS.Divide(hSigOS)
      
       #if(i==0 or i==5 or i==10 or i==15): 
       #for ix in range (0,hDataOS.GetXaxis().GetNbins()+1):
       #   for iy in range (0,hDataOS.GetYaxis().GetNbins()+1):
       #      for iz in range (0,hDataOS.GetZaxis().GetNbins()+1):
       #          #bin x 50 bin y 0 bin z 29
       #         if(hDataOS.GetBinContent(ix,iy,iz)<0):
       #            print "bin x "+str(ix)+" bin y "+str(iy)+" bin z "+str(iz)+" data "+str(hDataOS.GetBinContent(ix,iy,iz))+" signal "+str(hSigOS.GetBinContent(ix,iy,iz))+" ratio "+ str(ratioOS.GetBinContent(ix,iy,iz))
    
       ratioOS.SetName(his[i])
       ratioOS.Write()
       #hDataOS.SetName(his[i]+"_dataOS")
       #hDataOS.Write()
       hDataOS.SetName(his[i]+"_dataOS")
       hDataOS.Write()
       hSigOS.SetName(his[i]+"_SigOS")
       hSigOS.Write()
       hData.SetName(his[i]+"_pureData")
       hData.Write()
       hTotBkg.SetName(his[i]+"_pureBkg")
       hTotBkg.Write()






