#!/usr/bin/env python
#from HttStyles import GetStyleHtt
#from HttStyles import MakeCanvas
import plotting as plot
import ROOT
import re
from array import array

def createAxisHists(n,src,xmin=0,xmax=499):
  result = []
  for i in range(0,n):
    res = src.Clone()
    res.Reset()
    res.SetTitle("")
    res.SetName("axis%(i)d"%vars())
    res.SetAxisRange(xmin,xmax)
    res.SetStats(0)
    result.append(res)
  return result
ROOT.gROOT.SetBatch(ROOT.kTRUE)

    #ztt_noMET_mm.inputs-sm-13TeV_NB.root
    #ztt_noMET_mm.inputs-sm-13TeV_NB.root
    #ztt_noMET_mm.inputs-sm-13TeV_NB.root
    #ztt_noMET_mm.inputs-sm-13TeV_NB.root
    #ztt_noMET_mm.inputs-sm-13TeV_NB.root
    #ztt_noMET_mm.inputs-sm-13TeV_NB.root

files     = ["ztt_mt.inputs-sm-13TeV.root"]#,"ztt_noMET_mm.pt.inputs-sm-13TeV_NB.root","ztt_noMET_mm.eta.inputs-sm-13TeV_NB.root"]
quantity  = ["mass"]#,"pt","eta"]
channels  = ["mt"]#,"mt","mt","mt"]
channels2 = ["mt"]#,"et","mt","tt"]
labels    = ["inclusive","0jet","1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","2bjet","MSSM_btag"]

ncat=len(labels)
nfiles=len(files)
width=[0.3,0.3,0.3,0.3]
pos=[3,3,3,3]
#width=[0.2,0.3,0.2,0.3]
#pos=[4,3,4,3]

c2=ROOT.TCanvas()
c2.cd()
plot.ModTDRStyle(r=0.06, l=0.12)
pads=plot.TwoPadSplit(0.29,0.010,0.035)

for f in range (0,nfiles):
   file=ROOT.TFile(files[f],"r")
   for i in range (0,ncat):
 
         Data=file.Get(channels[0]+"_"+labels[i]).Get("data_obs")
         QCD=file.Get(channels[0]+"_"+labels[i]).Get("QCD")
         W=file.Get(channels[0]+"_"+labels[i]).Get("W")
         VV=file.Get(channels[0]+"_"+labels[i]).Get("VV")
         TT=file.Get(channels[0]+"_"+labels[i]).Get("TT")
         ZLL=file.Get(channels[0]+"_"+labels[i]).Get("ZLL")
         ZTT=file.Get(channels[0]+"_"+labels[i]).Get("ZTT")
         VBFH125=file.Get(channels[0]+"_"+labels[i]).Get("VBFH125")
         GGH125=file.Get(channels[0]+"_"+labels[i]).Get("GGH125")
         EWKW=file.Get(channels[0]+"_"+labels[i]).Get("EWKW")

         VV.Add(W) #electroweak only
         VV.Add(EWKW)
         VBFH125.Add(GGH125)
         TotProc=QCD.Clone()
         TotProc.Add(VV)
         TotProc.Add(TT)
         TotProc.Add(ZLL)
         TotProc.Add(ZTT)
         TotProc.Add(VBFH125)

         QCD.SetFillColor(ROOT.TColor.GetColor(250,202,255))
         QCD.SetLineColor(ROOT.kBlack)
         VV.SetFillColor(ROOT.TColor.GetColor(222,90,106))
         VV.SetLineColor(ROOT.kBlack)
         TT.SetFillColor(ROOT.TColor.GetColor(155,152,204))
         TT.SetLineColor(ROOT.kBlack)
         ZTT.SetFillColor(ROOT.TColor.GetColor(248,206,104))
         ZTT.SetLineColor(ROOT.kBlack)
         ZLL.SetFillColor(ROOT.TColor.GetColor(100,192,232))
         ZLL.SetLineColor(ROOT.kBlack)
         VBFH125.SetFillColor(ROOT.kBlue)
         VBFH125.SetLineColor(ROOT.kBlack)
         QCD.SetMarkerSize(0) 
         VV.SetMarkerSize(0) 
         TT.SetMarkerSize(0) 
         ZLL.SetMarkerSize(0) 
         ZTT.SetMarkerSize(0) 
         VBFH125.SetMarkerSize(0) 
 
         Data.SetMarkerStyle(20)
         Data.SetMarkerSize(1.2)
         Data.SetMarkerColor(ROOT.TColor.GetColor(0,0,0))
         Data.SetLineColor(ROOT.TColor.GetColor(0,0,0))
         Data.SetLineWidth(2)
         TotProc.SetFillColor(ROOT.TColor.GetColor(0,0,0))
         TotProc.SetFillStyle(3002)
         TotProc.SetMarkerSize(0)
      #TotProc.SetFillColor(plot.CreateTransparentColor(12,1))

         stack=ROOT.THStack("stack","stack")
         stack.Add(VBFH125)
         stack.Add(QCD)
         stack.Add(VV)
         stack.Add(TT)
         stack.Add(ZLL)
         stack.Add(ZTT)

      #setup style related things
      #c2=MakeCanvas(labels[i],labels[i],750,750)
      #c2.cd()
      #pads=plot.TwoPadSplit(0.29,0.005,0.005)
         pads[0].cd()
         #pads[0].SetLogy()
         TotProc.Draw()
         Data.Draw()
         stack.Draw()
         xmax = 200#TotProc.GetXaxis().GetXmax()
         xmin = 0#TotProc.GetXaxis().GetXmin()
         axish = createAxisHists(2,TotProc,xmin,xmax)
      #print " f %d - cat %d - xmin %d - xmax %d " %(f,i,xmin,xmax)
      #print " TotProc - xmin %d - xmax %d " %(TotProc.GetXaxis().GetXmin(),TotProc.GetXaxis().GetXmax())
      #print " Data - xmin %d - xmax %d " %(Data.GetXaxis().GetXmin(),Data.GetXaxis().GetXmax())
      #axish = createAxisHists(2,TotProc,TotProc.GetXaxis().GetXmin(),TotProc.GetXaxis().GetXmax())
         #axish[0].GetXaxis().SetTitle("m_{#tau#tau} (GeV)")
         stack.GetXaxis().SetLimits(xmin,xmax)
         Data.Draw("esame")
         TotProc.Draw("e2same")

         axish[0].GetYaxis().SetTitle("Events")
         axish[1].GetYaxis().SetTitle("Obs/Exp")
         axish[1].GetYaxis().SetNdivisions(4)
         axish[0].GetXaxis().SetTitleSize(0)
         axish[0].GetXaxis().SetLabelSize(0)
         axish[0].GetYaxis().SetTitleOffset(1.2)
         axish[1].GetYaxis().SetTitleOffset(1.2)
         axish[0].SetMaximum(100*TotProc.GetMaximum())
         axish[0].SetMinimum(0.1)
         axish[0].Draw("axissame")
         axish[1].Draw("axissame")
      #print " axis - xmin %d - xmax %d " %(axish[0].GetXaxis().GetXmin(),axish[0].GetXaxis().GetXmax())
      #stack.Draw()
      #stack.GetXaxis().SetLimits(xmin,xmax)
      #Data.GetXaxis().SetLimits(xmin,xmax)
      #print " Data - xmin %d - xmax %d " %(Data.GetXaxis().GetXmin(),Data.GetXaxis().GetXmax())
      #print " TotProc - xmin %d - xmax %d " %(TotProc.GetXaxis().GetXmin(),TotProc.GetXaxis().GetXmax())
      #Data.Draw()
      #TotProc.Draw()
      #stack.Draw()
         axish[0].Draw()

         stack.Draw("hsame")
         Data.Draw("esame")
         TotProc.Draw("e2same")
         axish[0].Draw("axissame")
 
         #legend = plot.PositionedLegend(width[j],0.3,pos[j],0.03) #mumu
         legend = plot.PositionedLegend(0.30,0.30,3,0.03)
         legend.SetTextFont(42)
         legend.SetTextSize(0.025)
         legend.SetFillColor(0)
         legend.AddEntry(ZTT,"Z#rightarrow#tau#tau","f")
         legend.AddEntry(ZLL,"Z#rightarrowll","f")
         legend.AddEntry(TT,"ttbar+jets","f")
         legend.AddEntry(VV,"Electroweak","f")
         legend.AddEntry(QCD,"QCD","f")
         legend.AddEntry(VBFH125,"SM H#rightarrow#tau#tau, m_{H}=125 GeV","f")
         legend.AddEntry(Data,"Observed","ep")
         legend.AddEntry(TotProc,"Exp. Uncertainty","f")
	 #legend.Draw("same")
         latex = ROOT.TLatex()
         latex.SetNDC()
         latex.SetTextAngle(0)
         latex.SetTextColor(ROOT.kBlack)
         latex.SetTextSize(0.026)

      #CMS and lumi labels
         plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.15)
         plot.DrawCMSLogo(pads[0], 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 1.0)
         plot.DrawTitle(pads[0], "2.3 fb^{-1} (13 TeV)", 3);
  
      #add ratio plot
         DataForRatio = Data.Clone()
         DataForRatio.Divide(TotProc)
         ErrForRatio = TotProc.Clone()
         ErrForRatio.Divide(TotProc)
      #DataForRatio.SetFillColor(plot.CreateTransparentColor(12,1))
         DataForRatio.SetFillColor(ROOT.kBlack)
         pads[1].cd()
         pads[1].SetGrid(1,1)
         axish[1].Draw("axis")
         axish[1].SetMinimum(0.8)
         axish[1].SetMaximum(1.2)
         axish[0].GetXaxis().SetTitle("m_{vis} [GeV]")
         DataForRatio.SetMarkerSize(1.2)
         DataForRatio.SetMarkerStyle(20)
         DataForRatio.SetMarkerColor(ROOT.kBlack)
         DataForRatio.Draw("esame")
         ErrForRatio.SetFillStyle(3002)
         ErrForRatio.SetFillColor(ROOT.kBlack)
         ErrForRatio.SetMarkerSize(0)
         ErrForRatio.Draw("e2same")
  
         pads[0].cd()
         pads[0].GetFrame().Draw("same")
         pads[0].RedrawAxis()
  
      #l1=add_lumi()
      #l1.Draw("same")
      #l2=add_CMS()
      #l2.Draw("same")
  
         c2.SaveAs(channels[0]+"_"+channels2[0]+"_"+labels[i]+"_"+quantity[f]+".pdf")
         c2.SaveAs(channels[0]+"_"+channels2[0]+"_"+labels[i]+"_"+quantity[f]+".png")

