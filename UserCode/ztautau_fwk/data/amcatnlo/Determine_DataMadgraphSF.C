#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLegend.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>

std::vector<TH1F*> CreateVectorHisto(std::string ch, int n, int nbins, int minx, int maxx){
    std::vector<TH1F*> vhisto;

    for(int i=0;i<n;i++){
        stringstream streamname;
        string histname;
        streamname<<"tmp"<<i;
        streamname>>histname;
        TH1F *tmp =  new TH1F((histname+ch).c_str(),(histname+ch).c_str(),nbins,minx,maxx);
        vhisto.push_back(tmp);
    }
    return vhisto;
}

std::vector<TH1D*> GetProjectionX(TH2F *h, string s){
    std::vector<TH1D*> vhisto;
    int binx1,binx2;

    for(int i=0;i<h->GetYaxis()->GetNbins();i++){
        if(h->GetBinContent(1,i+1)==0) continue;
        else{
            binx1 = i;
            binx2 = i+1;
        }
        stringstream streamname;
        string histname;
        streamname<<"proj"<<i;
        streamname>>histname;
        TH1D * proj = (TH1D*)h->ProjectionX((histname+"_"+s).c_str(),binx1,binx2);
        proj->Scale(1./proj->Integral());
        vhisto.push_back(proj);
    }

    return vhisto;
}

std::vector<TGraphErrors*> GetErrorGraph(std::vector<TH1D*> vh){
    std::vector<TGraphErrors*> graph;

    int n = vh[0]->GetXaxis()->GetNbins();
    float x[n]; 
    float ex[n]; 
    float content;
    float error;

    for(int i=0;i<vh.size();i++){

        float y[n]; 
        float ey[n]; 
        TGraphErrors *g = NULL; 
        if(i==0){
            for(int j=0;j<vh[i]->GetXaxis()->GetNbins();j++){
                x[j] = vh[i]->GetBinCenter(j+1);
                ex[j] = 5.;
            }
        }
        content = 0;
        error   = 0;

        for(int j=0;j<vh[i]->GetXaxis()->GetNbins();j++){
            content = content + vh[i]->GetBinContent(j+1);
            error   = error*error + vh[i]->GetBinError(j+1)*vh[i]->GetBinError(j+1);
            y[j]    = content;
            ey[j]   = TMath::Sqrt(error); 
        }
        g = new TGraphErrors(n,x,y,ex,ey);
        graph.push_back(g);
    }
    return graph;
}

std::vector<TGraphErrors*> GetErrorGraphR(std::vector<TH1D*> vh){
    std::vector<TGraphErrors*> graph;

    int n = vh[0]->GetXaxis()->GetNbins();
    float y[n]; 
    float ey[n]; 
    float content;
    float error;

    for(int i=0;i<vh.size();i++){

        TGraphErrors *g = NULL; 
        if(i==0){
            for(int j=0;j<vh[i]->GetXaxis()->GetNbins();j++){
                y[j] = vh[i]->GetBinCenter(j+1);
                ey[j] = 5.;
            }
        }
        content = 0;
        error   = 0;
        float x[n]; 
        float ex[n]; 

        for(int j=0;j<vh[i]->GetXaxis()->GetNbins();j++){
            content = content + vh[i]->GetBinContent(j+1);
            error   = error*error + vh[i]->GetBinError(j+1)*vh[i]->GetBinError(j+1);
            x[j]    = content;
            ex[j]   = TMath::Sqrt(error); 
        }
        g = new TGraphErrors(n,x,y,ex,ey);
        graph.push_back(g);
    }
    return graph;
}
    
void SetStyle(std::vector<TGraphErrors*> vgMC, std::vector<TGraphErrors*> vgData){
    for(int i=0; i<vgMC.size(); i++){
        vgMC[i]->SetTitle("");
        vgData[i]->SetTitle("");
        vgMC[i]->SetMarkerColor(4);
        vgMC[i]->SetLineColor(4);
        vgData[i]->SetMarkerColor(6);
        vgData[i]->SetLineColor(6);
    }
}   
    
void SaveGraphs(std::vector<TGraphErrors*> vgMC,std::vector<TGraphErrors*> vgData, std::vector<string> vs){
    TLegend* leg = new TLegend(0.5,0.2,0.9,0.4,NULL,"brNDC");
    leg->SetFillColor(0);
    leg->SetTextSize(0.029);
    leg->SetTextFont(42);
    leg->SetBorderSize(0);
    leg->AddEntry(vgMC[0],"MC","lep");
    leg->AddEntry(vgData[0],"Data","lep");

    for(int i=0; i<vgMC.size(); i++){
        //if(i==1){
        TCanvas *d0 = new TCanvas();
        //vgMC[i]->GetYaxis()->SetRangeUser(0,1);
        vgMC[i]->GetYaxis()->SetTitle("CDF");
        vgMC[i]->Draw("APC");
        vgData[i]->Draw("PC same");
        leg->Draw("same");
        d0->SaveAs(("graph_"+vs.at(i)+".png").c_str());
        d0->SaveAs(("graph_"+vs.at(i)+".root").c_str());
        //}
    } 
}
    
std::vector<TGraphErrors*> DoRatioPlot(std::vector<TGraphErrors*> vgMC, std::vector<TGraphErrors*> vgData, std::vector<TH1D*> vh){

    std::vector<TGraphErrors*> graph;

    int n = vh[0]->GetXaxis()->GetNbins();
    float x[n];
    float ex[n];

    for(int i=0;i<vh.size();i++){

        TGraphErrors *g = NULL;
            for(int j=0;j<vh[i]->GetXaxis()->GetNbins();j++){
                x[j] = vh[i]->GetBinCenter(j+1);
                ex[j] = 5.;
            }
        float y[n];
        float ey[n];

        for(int j=0;j<vh[i]->GetXaxis()->GetNbins();j++){
            float CDF_mc              = vgMC[i]->Eval(x[j]);
            float pt_corrected_data   = vgData[i]->Eval(CDF_mc);
            float e_pt_corrected_data = vgData[i]->GetErrorY(CDF_mc);
            y[j]  = pt_corrected_data/x[j];
            ey[j] = TMath::Sqrt( TMath::Power(e_pt_corrected_data/x[j],2) + TMath::Power((pt_corrected_data*ex[j])/(x[j]*x[j]),2) );
       
            cout << " mc pt " << x[j] << " and CDF is " << CDF_mc << endl;
            cout << " pt corrected is " << pt_corrected_data << " and ratio is " << y[j] << endl;
        }
        g = new TGraphErrors(n,x,y,ex,ey);  
        graph.push_back(g); 
    }   

    return graph;
}

double ComputeIntegral3D(TH3F *h){
    double integral=0;
    for(int ix = 0; ix<=h->GetXaxis()->GetNbins(); ix++){
        for(int iy = 0; iy<=h->GetYaxis()->GetNbins(); iy++){
            for(int iz = 0; iz<=h->GetZaxis()->GetNbins(); iz++){
                cout << " ix, iy, iz " << ix << ", " << iy << ", " << iz << endl;
                integral+=h->GetBinContent(ix,iy,iz);
            }
        }
    }
    return integral;
}

void Determine_DataMadgraphSF(){

    string ntupleDir = "../ntuples/mm_analysis_compute_weights_17_June/";
    std::vector<string> histos;
    histos.push_back("rweight_3d_EM");
    histos.push_back("rweight_3d_ET");
    histos.push_back("rweight_3d_MT");
    histos.push_back("rweight_3d_TT");
    //histos.push_back("rweight_3d_mt");

    TFile *fZTT    = new TFile((ntupleDir+"ZTT.root").c_str(),"r");  //
    TFile *fZLL    = new TFile((ntupleDir+"ZLL.root").c_str(),"r"); 
    TFile *fW      = new TFile((ntupleDir+"W.root").c_str(),"r");    //
    TFile *fTT     = new TFile((ntupleDir+"TT.root").c_str(),"r");   //
    TFile *fVV     = new TFile((ntupleDir+"VV.root").c_str(),"r");   //
    TFile *fGG     = new TFile((ntupleDir+"GG.root").c_str(),"r");   //
    TFile *fVBF    = new TFile((ntupleDir+"VBF.root").c_str(),"r");  //
    TFile *fEWKW   = new TFile((ntupleDir+"EWKW.root").c_str(),"r"); //
    TFile *fData   = new TFile((ntupleDir+"data_obs.root").c_str(),"r");
  


    TFile *fout = new TFile("TEST_reweighting_1bin_pt_1bin_mass_50_bin_eta.root","recreate");

    for(size_t ihis=0; ihis<histos.size(); ihis++){
            TH3F * hDataOS    = (TH3F*)fData->Get((histos.at(ihis)+"_data_obs").c_str());
            
            TH3F * hTotBkgOS  = (TH3F*)fVV->Get((histos.at(ihis)+"_VV").c_str());
            hTotBkgOS->Add((TH3F*)fEWKW->Get((histos.at(ihis)+"_EWKW").c_str()));
            hTotBkgOS->Add((TH3F*)fZTT->Get((histos.at(ihis)+"_ZTT").c_str()));
            hTotBkgOS->Add((TH3F*)fVBF->Get((histos.at(ihis)+"_VBF").c_str()));
            hTotBkgOS->Add((TH3F*)fTT->Get((histos.at(ihis)+"_TT").c_str()));
            hTotBkgOS->Add((TH3F*)fGG->Get((histos.at(ihis)+"_GG").c_str()));
            hTotBkgOS->Add((TH3F*)fW->Get((histos.at(ihis)+"_W").c_str()));
            

            TH3F * hSignalOS  = (TH3F*)fZLL->Get((histos.at(ihis)+"_ZLL").c_str());

            TH3F * hData = (TH3F*)hDataOS->Clone();
            hData->Add(hTotBkgOS,-1);
            TH3F * hRatio = (TH3F*)hData->Clone();
            hRatio->Divide(hSignalOS);
    
            Int_t xfirst = hData->GetXaxis()->GetFirst();
            Int_t xlast  = hData->GetXaxis()->GetLast();
            Int_t yfirst = hData->GetYaxis()->GetFirst();
            Int_t ylast  = hData->GetYaxis()->GetLast();
            Int_t zfirst = hData->GetZaxis()->GetFirst();
            Int_t zlast  = hData->GetZaxis()->GetLast();

            //cout << "fXaxis.GetFirst(),fXaxis.GetLast() " << hData->GetXaxis()->GetFirst() << "," << hData->GetXaxis()->GetLast() << endl;
            //cout << "fYaxis.GetFirst(),fYaxis.GetLast() " << hData->GetYaxis()->GetFirst() << "," << hData->GetYaxis()->GetLast() << endl;
            //cout << "fZaxis.GetFirst(),fZaxis.GetLast() " << hData->GetZaxis()->GetFirst() << "," << hData->GetZaxis()->GetLast() << endl;

            cout << " data-bkg integral " << hData->Integral() << endl;
            double integral = ComputeIntegral3D(hData); 
            cout << " data-bkg integral from the function " << integral << endl;
             
            hSignalOS->Scale(1./hSignalOS->Integral());
            hData->Scale(1./hData->Integral());
            
            TH3F * hRatioS = (TH3F*)hData->Clone();
            hRatioS->Divide(hSignalOS);

            cout << " AFTER normalizing to 1 " << endl;
            cout << " data-bkg integral " << hData->Integral() << endl;

            

    }

/*
                hData   = hDataOS.Clone()
                hTotBkg = hTotBkgOS.Clone()

                hDataOS.Add(hTotBkgOS,-1)
                hSigOS  = fZLL.Get(his+"_ZLL")

                hData.Scale(1./hData.Integral())
                hTotBkg.Scale(1./hTotBkg.Integral())
                hDataOS.Scale(1./hDataOS.Integral())
                hSigOS.Scale(1./hSigOS.Integral())
*/


}
