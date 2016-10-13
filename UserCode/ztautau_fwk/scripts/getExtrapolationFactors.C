#include "TH1.h"
#include "TMath.h"

void getExtrapolationFactors(){

    TFile *fmt   = new TFile("ANALYSIS_ztt_mt.inputs-sm-13TeV_2D.root","r");
    TFile *fmm   = new TFile("ANALYSIS_ztt_mm.inputs-sm-13TeV_2D.root","r");
    //TFile *fmme_ = new TFile("ztt_mm_woTauSpinner.inputs-sm-13TeV.root","r");
    TFile *fmme  = new TFile("ANALYSIS_ztt_mm_wTauSpinner.inputs-sm-13TeV_2D.root","r");

    TString input_dir[]={"0jet","1jet_zpt_loose","1jet_zpt_medium","1jet_zpt_tight","2jet_cp","vbf","1bjet","2bjet","MSSM_btag"};//,"1jet","2jets"};
    int ncat = 9;

    TH1F * h_inclusive_mt   = (TH1F*)fmt->Get("mt_inclusive/ZTT");
    TH1F * h_inclusive_mm   = (TH1F*)fmm->Get("mm_inclusive/ZLL");
    //TH1F * h_inclusive_mme_ = (TH1F*)fmme_->Get("mm_inclusive_embedding/ZLL");
    TH1F * h_inclusive_mme  = (TH1F*)fmme->Get("mm_inclusive_embedding/ZLL");
    h_inclusive_mt->Rebin(h_inclusive_mt->GetXaxis()->GetNbins());
    h_inclusive_mm->Rebin(h_inclusive_mm->GetXaxis()->GetNbins());
    //h_inclusive_mme_->Rebin(h_inclusive_mme_->GetXaxis()->GetNbins());
    h_inclusive_mme->Rebin(h_inclusive_mme->GetXaxis()->GetNbins());

    for(int i=0;i<ncat;i++){

        TH1F * h_exclusive_mt   = (TH1F*)fmt->Get("mt_"+input_dir[i]+"/ZTT");
        TH1F * h_exclusive_mm   = (TH1F*)fmm->Get("mm_"+input_dir[i]+"/ZLL");
        //TH1F * h_exclusive_mme_ = (TH1F*)fmme_->Get("mm_"+input_dir[i]+"_embedding/ZLL");
        TH1F * h_exclusive_mme  = (TH1F*)fmme->Get("mm_"+input_dir[i]+"_embedding/ZLL");

        h_exclusive_mt->Rebin(h_exclusive_mt->GetXaxis()->GetNbins());
        h_exclusive_mm->Rebin(h_exclusive_mm->GetXaxis()->GetNbins());
        //h_exclusive_mme_->Rebin(h_exclusive_mme_->GetXaxis()->GetNbins());
        h_exclusive_mme->Rebin(h_exclusive_mme->GetXaxis()->GetNbins());

        double mt_ratio  = h_exclusive_mt->GetBinContent(1)/h_inclusive_mt->GetBinContent(1);
        
        double mt_err    = TMath::Sqrt( TMath::Power(h_exclusive_mt->GetBinError(1)/h_inclusive_mt->GetBinContent(1),2) + TMath::Power((h_exclusive_mt->GetBinContent(1)*h_inclusive_mt->GetBinError(1))/TMath::Power(h_inclusive_mt->GetBinContent(1),2),2)  );
        
        double mm_ratio  = h_exclusive_mm->GetBinContent(1)/h_inclusive_mm->GetBinContent(1);

        double mm_err    = TMath::Sqrt( TMath::Power(h_exclusive_mm->GetBinError(1)/h_inclusive_mm->GetBinContent(1),2) + TMath::Power((h_exclusive_mm->GetBinContent(1)*h_inclusive_mm->GetBinError(1))/TMath::Power(h_inclusive_mm->GetBinContent(1),2),2)  );

        /*double mme__ratio = h_exclusive_mme_->GetBinContent(1)/h_inclusive_mme_->GetBinContent(1);

        double mme__err    = TMath::Sqrt( TMath::Power(h_exclusive_mme_->GetBinError(1)/h_inclusive_mme_->GetBinContent(1),2) + TMath::Power((h_exclusive_mme_->GetBinContent(1)*h_inclusive_mme_->GetBinError(1))/TMath::Power(h_inclusive_mme_->GetBinContent(1),2),2)  );
        */

        double mme_ratio = h_exclusive_mme->GetBinContent(1)/h_inclusive_mme->GetBinContent(1);

        double mme_err    = TMath::Sqrt( TMath::Power(h_exclusive_mme->GetBinError(1)/h_inclusive_mme->GetBinContent(1),2) + TMath::Power((h_exclusive_mme->GetBinContent(1)*h_inclusive_mme->GetBinError(1))/TMath::Power(h_inclusive_mme->GetBinContent(1),2),2)  );
        cout <<  input_dir[i] << endl;
        cout << " mm: " << mm_ratio << " pm " << mm_err << " mt: " << mt_ratio << " pm " << mt_err <<
            " mm embedding w tau spinner " << mme_ratio << " pm " << mme_err << endl;

    }
}
