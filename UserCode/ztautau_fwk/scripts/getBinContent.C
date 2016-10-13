#include "TH1.h"
#include "TH3.h"
#include "TMath.h"
#include "TCanvas.h"
#include <math.h>
void getBinContent(){

    /*
       datacards_250516_wo_OVERFLOW/ztt_mm.inputs-sm-13TeV_checks_3D_DataMG_1bin_eta.root
       datacards_250516_wo_OVERFLOW/ztt_mm.inputs-sm-13TeV_checks_3D_DataMG_eta.root
       datacards_250516_wo_OVERFLOW/ztt_mm.inputs-sm-13TeV_checks_3D_eta.root
    */

    TFile *file_wo_weight = new TFile("ztt_mm.inputs-sm-13TeV_checks_3D_pt.root","r");
    TFile *file_w_weight  = new TFile("ztt_mm.inputs-sm-13TeV_checks_3D_DataMG_1bin_pt_pt.root","r");
    TFile *file_m_weight  = new TFile("../data/amcatnlo/reweighting.root","r");

    TH3F *h_matrix_weight = (TH3F*)file_m_weight->Get("rweight_3d_mm_1bin_pt");
    TH1F* h_weight        = new TH1F("h_weight","h_weight",100,-1,5);

    for(size_t i = 0; i <= h_matrix_weight->GetZaxis()->GetNbins()+1; i++){
        h_weight->Fill(h_matrix_weight->GetBinContent(1,1,i));
    }

    //TCanvas *c = new TCanvas();
    //h_weight->Draw();

    TH1F *data = (TH1F*)file_wo_weight->Get("mm_inclusive/data_obs"); 
    TH1F *VV   = (TH1F*)file_wo_weight->Get("mm_inclusive/VV");
    TH1F *TT   = (TH1F*)file_wo_weight->Get("mm_inclusive/TT");   
    TH1F *ZTT  = (TH1F*)file_wo_weight->Get("mm_inclusive/ZTT");  
    TH1F *W    = (TH1F*)file_wo_weight->Get("mm_inclusive/W");    
    TH1F *VBF  = (TH1F*)file_wo_weight->Get("mm_inclusive/VBF");  
    TH1F *GG   = (TH1F*)file_wo_weight->Get("mm_inclusive/GG");   
    TH1F *EWKW = (TH1F*)file_wo_weight->Get("mm_inclusive/EWKW"); 
    
    TH1F *ZLL_wo_W  = (TH1F*)file_wo_weight->Get("mm_inclusive/ZLL");      TH1F *ZLL_w_W = (TH1F*)file_w_weight->Get("mm_inclusive/ZLL");
    TH1F *QCD_wo_W  = (TH1F*)file_wo_weight->Get("mm_inclusive/QCD");      TH1F *QCD_w_W = (TH1F*)file_w_weight->Get("mm_inclusive/QCD");
    TH1F *ZLL_wo_W_normalized = (TH1F*)ZLL_wo_W->Clone();
    ZLL_wo_W_normalized->Scale(1./ZLL_wo_W_normalized->Integral());
        
    TH1F *totBkg_without_QCD = (TH1F*)VV->Clone();
    totBkg_without_QCD->Add(TT);  totBkg_without_QCD->Add(ZTT); totBkg_without_QCD->Add(W); 
    totBkg_without_QCD->Add(VBF); totBkg_without_QCD->Add(GG);  totBkg_without_QCD->Add(EWKW);

    TH1F *totBkg_with_QCD_wo_W = (TH1F*)totBkg_without_QCD->Clone();
    totBkg_without_QCD->Add(QCD_wo_W);
    
    TH1F *totBkg_with_QCD_w_W  = (TH1F*)totBkg_without_QCD->Clone();
    totBkg_without_QCD->Add(QCD_w_W);

    TH1F *totData = (TH1F*)data->Clone();
    totData->Add(totBkg_with_QCD_wo_W,-1);
    totData->Scale(1./totData->Integral());
 
    cout << " bin content data-bkg normalized " << totData->GetBinContent(2) << endl; ///ZLL_wo_W_normalized->GetBinContent(2) << endl;
    cout << " bin content ZLL normalized " << ZLL_wo_W_normalized->GetBinContent(2) << endl; ///ZLL_wo_W_normalized->GetBinContent(2) << endl;


    cout << " START printing for CLOSURE test " << endl;
    cout << " 1)  data events                                                  " << data->GetBinContent(2) << endl;
    cout << " 2a) tot. bkg events (without QCD)                                " << totBkg_without_QCD->GetBinContent(2) << endl;
    cout << " 2b) tot. bkg events (with QCD, no weight)                        " << totBkg_with_QCD_wo_W->GetBinContent(2) << endl;
    cout << " 3)  ZLL events (no weight)                                       " << ZLL_wo_W->GetBinContent(2) << endl;
    cout << " 4a) data-tot. bkg events events (without QCD)                    " << data->GetBinContent(2)-totBkg_without_QCD->GetBinContent(2) << endl;
    cout << " 4b) data-tot. bkg events events (with QCD, no weight)            " << data->GetBinContent(2)-totBkg_with_QCD_wo_W->GetBinContent(2) << endl;
    cout << " 5a) data-tot. bkg events events (without QCD) - all ETA          " << data->Integral()-totBkg_without_QCD->Integral() << endl;
    cout << " 5b) data-tot. bkg events events (with QCD, no weight) - all ETA  " << data->Integral()-totBkg_with_QCD_wo_W->Integral() << endl;
    cout << " 6)  ZLL events (no weight)                                       " << ZLL_wo_W->GetBinContent(2) << endl;
    cout << " 7)  ZLL events - all ETA                                         " << ZLL_wo_W->Integral() << endl;

    double number_4a = data->GetBinContent(2)-totBkg_without_QCD->GetBinContent(2);
    double number_4b = data->GetBinContent(2)-totBkg_with_QCD_wo_W->GetBinContent(2);
    double number_5a = data->Integral()-totBkg_without_QCD->Integral();
    double number_5b = data->Integral()-totBkg_with_QCD_wo_W->Integral();
    double number_6  = ZLL_wo_W->GetBinContent(2) ;
    double number_7 = ZLL_wo_W->Integral(); 
    
    cout << " [(4a) / (5a)] / [ (6) / (7) ] " << ( number_4a/number_5a ) / ( number_6/number_7 ) << endl; 
    cout << " [(4b) / (5b)] / [ (6) / (7) ] " << ( number_4b/number_5b ) / ( number_6/number_7 ) << endl; 




}
