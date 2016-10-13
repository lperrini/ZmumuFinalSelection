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
        vgData[i]->Draw("PC diff");
        leg->Draw("diff");
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



void CreateFloatingBinning(){

    TFile *fZLL   = new TFile("file_floating_bins_ZLL.root","r");
    TFile *fData  = new TFile("file_floating_bins_Data.root","r");

    TH1D * projX_ZLL  = (TH1D*)fZLL->Get("zpt_ZLL");
    TH1D * projY_ZLL  = (TH1D*)fZLL->Get("zmass_ZLL");
    TH1D * projZ_ZLL  = (TH1D*)fZLL->Get("zeta_ZLL");
    
    TH1D * projX_Data  = (TH1D*)fData->Get("zpt_data_obs");
    TH1D * projY_Data  = (TH1D*)fData->Get("zmass_data_obs");
    TH1D * projZ_Data  = (TH1D*)fData->Get("zeta_data_obs");

    int nbinsX = projX_ZLL->GetXaxis()->GetNbins();
    int nbinsY = projY_ZLL->GetXaxis()->GetNbins();
    int nbinsZ = projZ_ZLL->GetXaxis()->GetNbins();
    
    projX_ZLL->Scale(1./projX_ZLL->Integral(1,nbinsX));
    projY_ZLL->Scale(1./projY_ZLL->Integral(1,nbinsY));
    projZ_ZLL->Scale(1./projZ_ZLL->Integral(1,nbinsZ));
    
    projX_Data->Scale(1./projX_Data->Integral(1,nbinsX));
    projY_Data->Scale(1./projY_Data->Integral(1,nbinsY));
    projZ_Data->Scale(1./projZ_Data->Integral(1,nbinsZ));

    double contentX_ZLL    = 0;
    double contentP_ZLL    = 0;
    double contentY_ZLL    = 0;
    double contentM_ZLL    = 0;
    double contentZ_ZLL    = 0;
    double contentE_ZLL    = 0;
    double contentX_Data   = 0;
    double contentP_Data   = 0;
    double contentY_Data   = 0;
    double contentM_Data   = 0;
    double contentZ_Data   = 0;
    double contentE_Data   = 0;
    double percentage      = 0.02;

    std::vector<double> vBinPt;
    vBinPt.push_back(0);
    std::vector<double> vBinM;
    vBinM.push_back(0);
    std::vector<double> vBinEta;
    vBinEta.push_back(-10);
    
//cout << "PERCENTAGE " << percentage_ZLL << " ZLL bin --> " << ix_ZLL+1 << " bin center is " << projX_ZLL->GetBinCenter(ix_ZLL+1) << " total content " << contentX_ZLL << endl;

    for(int ix=0; ix<nbinsX; ix++){
        contentX_ZLL  = contentX_ZLL+projX_ZLL->GetBinContent(ix+1);
        contentX_Data = contentX_Data+projX_Data->GetBinContent(ix+1);
        if(contentX_ZLL/percentage>=1 && contentX_Data/percentage>=1){
            vBinPt.push_back(projX_ZLL->GetBinCenter(ix+1)+projX_ZLL->GetBinWidth(ix+1)/2);
            contentP_ZLL=contentP_ZLL+contentX_ZLL;
            contentP_Data=contentP_Data+contentX_Data;
            contentX_ZLL=0.0;
            contentX_Data=0.0;
        }
    }

    vBinPt[vBinPt.size()-1]=1000;

    for(int iy=0; iy<nbinsY; iy++){
        contentY_ZLL  = contentY_ZLL+projY_ZLL->GetBinContent(iy+1);
        contentY_Data = contentY_Data+projY_Data->GetBinContent(iy+1);
        if(contentY_ZLL/percentage>=1 && contentY_Data/percentage>=1){
            vBinM.push_back(projY_ZLL->GetBinLowEdge(iy+1)+projY_ZLL->GetBinWidth(iy+1));
            //vBinM.push_back(projY_ZLL->GetBinCenter(iy+1)+projY_ZLL->GetBinWidth(iy+1)/2);
            contentM_ZLL=contentM_ZLL+contentY_ZLL;
            contentM_Data=contentM_Data+contentY_Data;
            contentY_ZLL=0.0;
            contentY_Data=0.0;
        }
    }
    
    vBinM[vBinM.size()-1]=1000;
    
    for(int iz=0; iz<nbinsZ; iz++){
        contentZ_ZLL  = contentZ_ZLL+projZ_ZLL->GetBinContent(iz+1);
        contentZ_Data = contentZ_Data+projZ_Data->GetBinContent(iz+1);
        //if(contentZ_Data/percentage>=1){
        if(contentZ_ZLL/percentage>=1 && contentZ_Data/percentage>=1){
            vBinEta.push_back(projZ_ZLL->GetBinCenter(iz+1)+projZ_ZLL->GetBinWidth(iz+1)/2);
            contentE_ZLL=contentE_ZLL+contentZ_ZLL;
            contentE_Data=contentE_Data+contentZ_Data;
            contentZ_ZLL=0.0;
            contentZ_Data=0.0;
        }
    }
    
    vBinEta[vBinEta.size()-1]=10;


    cout << "pt n bins " << vBinPt.size() << " content ZLL " << contentP_ZLL << " data " << contentP_Data << endl;
    cout << "M n bins " << vBinM.size() << " content ZLL " << contentM_ZLL << " data " << contentM_Data << endl;
    cout << "eta n bins " << vBinEta.size() << " content ZLL " << contentE_ZLL << " data " << contentE_Data << endl;
    
    cout << "*******PT "<< endl;
    for(int ix=0;ix<vBinPt.size();ix++){
    cout << vBinPt.at(ix) << ", ";
    } 
    
    cout << "\n *******MASS "<< endl;
    for(int iy=0;iy<vBinM.size();iy++){
    cout << vBinM.at(iy) << ", ";
    } 
    
    cout << "\n *******ETA "<< endl;
    for(int iz=0;iz<vBinEta.size();iz++){
    cout << vBinEta.at(iz) << ", ";
    } 

    cout << "\n "<< endl;
}
