#include "TH1.h"
#include "TH2.h"
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



void ChecksOnWeights(){

    TFile *f1             = new TFile("reweighting.root","r");//_1bin_same_codeA_codeB.root","r");
    TH2F *hmm             = (TH2F*)f1->Get("");
    TH2F *hmt             = (TH2F*)f1->Get("rweight_3d_mt");
    TH2F *hmm_data        = (TH2F*)f1->Get("rweight_3d_mm_dataOS");
    TH2F *hmm_MC          = (TH2F*)f1->Get("rweight_3d_mm_SigOS");
    TH2F *hmt_data        = (TH2F*)f1->Get("rweight_3d_mt_dataOS");
    TH2F *hmt_MC          = (TH2F*)f1->Get("rweight_3d_mt_SigOS");
    
        //reweighting_muta_3d->Fill(ptvis,mvis,etavis,fweight); 
    cout << " x bin " << hmm_data->GetXaxis()->GetNbins() << endl;
    cout << " y bin " << hmm_data->GetYaxis()->GetNbins() << endl;
    cout << " z bin " << hmm_data->GetZaxis()->GetNbins() << endl;

    TH1F *h = new TH1F("h","h",50,0,5);
    int count=0;

    for(size_t ix = 0; ix <= hmm_data->GetXaxis()->GetNbins(); ix++){
        for(size_t iy = 0; iy <= hmm_data->GetYaxis()->GetNbins(); iy++){
            for(size_t iz = 0; iz <= hmm_data->GetZaxis()->GetNbins(); iz++){
                cout << "bin pt/mass/eta " << ix << "/" << iy << "/" << iz << " data is: "<< hmm_data->GetBinContent(ix,iy,iz) << " so final weigh is " << hmm->GetBinContent(ix,iy,iz) << endl; 
                    //h->Fill(hmm->GetBinContent(1,1,i+1));
            }
        }
    }

    cout << count << endl;
    //TCanvas *c = new TCanvas();
    //h->Draw();

    /*std::vector<TH1D*> projx_mm_MC        = GetProjectionX(hmm_MC,"mm_MC");
    std::vector<TH1D*> projx_mm_data      = GetProjectionX(hmm_data,"mm_data");
    std::vector<TH1D*> projx_mt_MC        = GetProjectionX(hmt_MC,"mt_MC");
    std::vector<TH1D*> projx_mt_data      = GetProjectionX(hmt_data,"mt_data");

    std::vector<TGraphErrors*> gr_mm_MC        = GetErrorGraph(projx_mm_MC);
    std::vector<TGraphErrors*> gr_mm_data      = GetErrorGraph(projx_mm_data);
    std::vector<TGraphErrors*> gr_mt_MC        = GetErrorGraph(projx_mt_MC);
    std::vector<TGraphErrors*> gr_mt_data      = GetErrorGraph(projx_mt_data);
    std::vector<TGraphErrors*> gr_mm_data_R    = GetErrorGraphR(projx_mm_data);
    std::vector<TGraphErrors*> gr_mt_data_R    = GetErrorGraphR(projx_mt_data);
    
    int nbins_mm = 20; //non empty mass bins 19, from 11 to 30
    int nbins_mt = 10;  //non empty mass bins 9, from 6 to 15
    std::vector<string> mm_string;
    std::vector<string> mt_string;
    std::vector<float> dimuon_binning;
    int j=5;
    for(int i=0;i<50;i++){
        dimuon_binning.push_back(j);
        j=j+10;
    }

    mm_string.push_back("50_55");   mm_string.push_back("55_60");   mm_string.push_back("60_65");   mm_string.push_back("65_70");   mm_string.push_back("70_75"); 
    mm_string.push_back("75_80");   mm_string.push_back("80_85");   mm_string.push_back("85_90");   mm_string.push_back("90_95");   mm_string.push_back("95_100");
    mm_string.push_back("100_105"); mm_string.push_back("105_110"); mm_string.push_back("110_115"); mm_string.push_back("115_120"); 
    mm_string.push_back("120_125"); mm_string.push_back("125_130"); mm_string.push_back("130_135"); mm_string.push_back("135_140"); 
    mm_string.push_back("140_145"); mm_string.push_back("145_150");
    mt_string.push_back("50_60");   mt_string.push_back("60_70");   mt_string.push_back("70_80"); mt_string.push_back("80_90"); 
    mt_string.push_back("90_100");  mt_string.push_back("100_110"); mt_string.push_back("110_120"); 
    mt_string.push_back("120_130"); mt_string.push_back("130_140"); mt_string.push_back("140_150");
 
    
    SetStyle(gr_mm_MC,gr_mm_data);  
    SetStyle(gr_mt_MC,gr_mt_data);
    SaveGraphs(gr_mm_MC,gr_mm_data,mm_string);
    SaveGraphs(gr_mt_MC,gr_mt_data,mt_string);

    cout << " mm " << endl;
    std::vector<TGraphErrors*> gr_ratio_mm = DoRatioPlot(gr_mm_MC,gr_mm_data_R,projx_mm_MC);
    cout << " mt " << endl;
    std::vector<TGraphErrors*> gr_ratio_mt = DoRatioPlot(gr_mt_MC,gr_mt_data_R,projx_mt_MC);

    std::vector<float> p0values;
    std::vector<float> p1values;
    TH1F *p0distribution = new TH1F("hp0","hp0",10,0,2);
    TH1F *p1distribution = new TH1F("hp1","hp1",10,-0.001,0.001);
    for(int i=0; i<gr_ratio_mm.size(); i++){
        TCanvas *d0 = new TCanvas();
        gr_ratio_mm[i]->GetYaxis()->SetTitle("corrected data p_{T}/MC p_{T}");
        gr_ratio_mm[i]->GetXaxis()->SetTitle("dimuon p_{T} (GeV)");
        gr_ratio_mm[i]->SetTitle("");
        gr_ratio_mm[i]->Fit("pol1");
        //gr_ratio_mm[i]->Draw("AP");
        //d0->SaveAs(("ratio_graph_"+mm_string.at(i)+".png").c_str());
        TF1 *myfit = (TF1*) gr_ratio_mm[i]->GetFunction("pol1");
        p0values.push_back(myfit->GetParameter(0));
        p1values.push_back(myfit->GetParameter(1));
        p0distribution->Fill(myfit->GetParameter(0));
        p1distribution->Fill(myfit->GetParameter(1));
    }

    TCanvas *c1 = new TCanvas();
    p0distribution->Draw();
    TCanvas *c2 = new TCanvas();
    p1distribution->Draw();

    std::vector<TH1F*> vhisto_mm_pt = CreateVectorHisto("mm_pt",nbins_mm,50,0,500);
    std::vector<TH1F*> vhisto_mt_pt = CreateVectorHisto("mt_pt",nbins_mt,50,0,500);
    int count_mm=0;
    int count_mt=0;

    for(int iy=10;iy<hmm->GetYaxis()->GetNbins();iy++){
        for(int ix=0;ix<hmm->GetXaxis()->GetNbins();ix++){
            vhisto_mm_pt[count_mm]->SetBinContent(ix+1,hmm->GetBinContent(ix+1,iy+1));
            vhisto_mm_pt[count_mm]->SetBinError(ix+1,hmm->GetBinError(ix+1,iy+1));
        }
        vhisto_mm_pt[count_mm]->GetYaxis()->SetRangeUser(0,15); 
        count_mm++;
    }
    for(int iy=5;iy<hmt->GetYaxis()->GetNbins();iy++){
        for(int ix=0;ix<hmt->GetXaxis()->GetNbins();ix++){
            vhisto_mt_pt[count_mt]->SetBinContent(ix+1,hmt->GetBinContent(ix+1,iy+1));
            vhisto_mt_pt[count_mt]->SetBinError(ix+1,hmt->GetBinError(ix+1,iy+1));
        } 
        vhisto_mt_pt[count_mt]->GetYaxis()->SetRangeUser(0,15); 
        count_mt++;
    }
    */
/*    TCanvas *c0 = new TCanvas(); vhisto_mt_pt[0]->Draw(); vhisto_mt_pt[0]->Fit("pol1"); c0->SaveAs("mt_ptbins_mass_50_60.png");
    TCanvas *c1 = new TCanvas(); vhisto_mt_pt[1]->Draw(); vhisto_mt_pt[1]->Fit("pol1"); c1->SaveAs("mt_ptbins_mass_60_70.png");
    TCanvas *c2 = new TCanvas(); vhisto_mt_pt[2]->Draw(); vhisto_mt_pt[2]->Fit("pol1"); c2->SaveAs("mt_ptbins_mass_70_80.png");
    TCanvas *c3 = new TCanvas(); vhisto_mt_pt[3]->Draw(); vhisto_mt_pt[3]->Fit("pol1"); c3->SaveAs("mt_ptbins_mass_80_90.png");
    TCanvas *c4 = new TCanvas(); vhisto_mt_pt[4]->Draw(); vhisto_mt_pt[4]->Fit("pol1"); c4->SaveAs("mt_ptbins_mass_90_100.png");
    TCanvas *c5 = new TCanvas(); vhisto_mt_pt[5]->Draw(); vhisto_mt_pt[5]->Fit("pol1"); c5->SaveAs("mt_ptbins_mass_100_110.png");
    TCanvas *c6 = new TCanvas(); vhisto_mt_pt[6]->Draw(); vhisto_mt_pt[6]->Fit("pol1"); c6->SaveAs("mt_ptbins_mass_110_120.png");
    TCanvas *c7 = new TCanvas(); vhisto_mt_pt[7]->Draw(); vhisto_mt_pt[7]->Fit("pol1"); c7->SaveAs("mt_ptbins_mass_120_130.png");
    TCanvas *c8 = new TCanvas(); vhisto_mt_pt[8]->Draw(); vhisto_mt_pt[8]->Fit("pol1"); c8->SaveAs("mt_ptbins_mass_130_140.png");
    TCanvas *c9 = new TCanvas(); vhisto_mt_pt[9]->Draw(); vhisto_mt_pt[9]->Fit("pol1"); c9->SaveAs("mt_ptbins_mass_140_150.png");

    TCanvas *c01  = new TCanvas(); vhisto_mm_pt[0]->Draw();  vhisto_mm_pt[0]->Fit("pol1");  c01->SaveAs("mm_ptbins_mass_50_55.png");
    TCanvas *c11  = new TCanvas(); vhisto_mm_pt[1]->Draw();  vhisto_mm_pt[1]->Fit("pol1");  c11->SaveAs("mm_ptbins_mass_55_60.png");
    TCanvas *c21  = new TCanvas(); vhisto_mm_pt[2]->Draw();  vhisto_mm_pt[2]->Fit("pol1");  c21->SaveAs("mm_ptbins_mass_60_65.png");
    TCanvas *c31  = new TCanvas(); vhisto_mm_pt[3]->Draw();  vhisto_mm_pt[3]->Fit("pol1");  c31->SaveAs("mm_ptbins_mass_65_70.png");
    TCanvas *c41  = new TCanvas(); vhisto_mm_pt[4]->Draw();  vhisto_mm_pt[4]->Fit("pol1");  c41->SaveAs("mm_ptbins_mass_70_75.png");
    TCanvas *c51  = new TCanvas(); vhisto_mm_pt[5]->Draw();  vhisto_mm_pt[5]->Fit("pol1");  c51->SaveAs("mm_ptbins_mass_75_80.png");
    TCanvas *c61  = new TCanvas(); vhisto_mm_pt[6]->Draw();  vhisto_mm_pt[6]->Fit("pol1");  c61->SaveAs("mm_ptbins_mass_80_85.png");
    TCanvas *c71  = new TCanvas(); vhisto_mm_pt[7]->Draw();  vhisto_mm_pt[7]->Fit("pol1");  c71->SaveAs("mm_ptbins_mass_85_90.png");
    TCanvas *c81  = new TCanvas(); vhisto_mm_pt[8]->Draw();  vhisto_mm_pt[8]->Fit("pol1");  c81->SaveAs("mm_ptbins_mass_90_95.png");
    TCanvas *c91  = new TCanvas(); vhisto_mm_pt[9]->Draw();  vhisto_mm_pt[9]->Fit("pol1");  c91->SaveAs("mm_ptbins_mass_95_100.png");
    TCanvas *c101 = new TCanvas(); vhisto_mm_pt[10]->Draw(); vhisto_mm_pt[10]->Fit("pol1"); c101->SaveAs("mm_ptbins_mass_100_105.png");
    TCanvas *c111 = new TCanvas(); vhisto_mm_pt[11]->Draw(); vhisto_mm_pt[11]->Fit("pol1"); c111->SaveAs("mm_ptbins_mass_105_110.png");
    TCanvas *c121 = new TCanvas(); vhisto_mm_pt[12]->Draw(); vhisto_mm_pt[12]->Fit("pol1"); c121->SaveAs("mm_ptbins_mass_110_115.png");
    TCanvas *c131 = new TCanvas(); vhisto_mm_pt[13]->Draw(); vhisto_mm_pt[13]->Fit("pol1"); c131->SaveAs("mm_ptbins_mass_115_120.png");
    TCanvas *c141 = new TCanvas(); vhisto_mm_pt[14]->Draw(); vhisto_mm_pt[14]->Fit("pol1"); c141->SaveAs("mm_ptbins_mass_120_125.png");
    TCanvas *c151 = new TCanvas(); vhisto_mm_pt[15]->Draw(); vhisto_mm_pt[15]->Fit("pol1"); c151->SaveAs("mm_ptbins_mass_125_130.png");
    TCanvas *c161 = new TCanvas(); vhisto_mm_pt[16]->Draw(); vhisto_mm_pt[16]->Fit("pol1"); c161->SaveAs("mm_ptbins_mass_130_135.png");
    TCanvas *c171 = new TCanvas(); vhisto_mm_pt[17]->Draw(); vhisto_mm_pt[17]->Fit("pol1"); c171->SaveAs("mm_ptbins_mass_135_140.png");
    TCanvas *c181 = new TCanvas(); vhisto_mm_pt[18]->Draw(); vhisto_mm_pt[18]->Fit("pol1"); c181->SaveAs("mm_ptbins_mass_140_145.png");
    TCanvas *c191 = new TCanvas(); vhisto_mm_pt[19]->Draw(); vhisto_mm_pt[19]->Fit("pol1"); c191->SaveAs("mm_ptbins_mass_145_150.png");
*/








}
