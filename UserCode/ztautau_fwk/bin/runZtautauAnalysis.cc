#include <iostream>
#include <vector>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>     
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>

#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TNtuple.h"
#include <TBenchmark.h>
#include <Math/VectorUtil.h>

#include "UserCode/ztautau_fwk/interface/MainTreeHandler.h"
#include "HTT-utilities/LepEffInterface/interface/ScaleFactor.h"
#include "MCEmbeddingWeights/MCEmbeddingWeights/interface/MCEmbeddingWeights.h"
#include "UserCode/ztautau_fwk/interface/BTagCalibrationStandalone.h"
#include "UserCode/ztautau_fwk/interface/bTagSF.h"
#include "UserCode/ztautau_fwk/interface/myHelper.h"

using namespace std;
using optutl::CommandLineParser;

TH1F *DrawOverflow(TH1F* h){
    //function to paint the histogram h with an extra bin for overflows
    UInt_t nx    = h->GetNbinsX()+1;
    Double_t *xbins= new Double_t[nx+1];
    for (UInt_t i=0;i<nx;i++)
        xbins[i]=h->GetBinLowEdge(i+1);
    xbins[nx]=xbins[nx-1]+h->GetBinWidth(nx);
    //book a temporary histogram having extra bins for overflows
    TH1F *htmp = new TH1F(h->GetName(), h->GetTitle(), nx, xbins);
    htmp->Sumw2();
    //fill the new histogram including the overflows
    for (UInt_t i=1; i<=nx; i++) {
        htmp->SetBinContent(htmp->FindBin(htmp->GetBinCenter(i)),h->GetBinContent(i));
        htmp->SetBinError(htmp->FindBin(htmp->GetBinCenter(i)),h->GetBinError(i));
    }
    htmp->SetBinContent(htmp->FindBin(h->GetBinLowEdge(1)-1), h->GetBinContent(0));
    htmp->SetBinError(htmp->FindBin(h->GetBinLowEdge(1)-1), h->GetBinError(0));
    // Restore the number of entries
    // the original macro says htmp->SetEntries(h->GetEntries()) but this is wrong!
    // GetEntries would be correct if you work with histogram filled with weight=1
    // if you work with weights!=1 you have to use GetEffectiveEntries() which returns 
    // the square of sum of the weights divided by the sum of the weights square
    htmp->SetEntries(h->GetEffectiveEntries());
    return htmp;
}
                            
TH1F * RebinHistogramFloatingBins(TH1F* h, int nbins, float bins[]){        
    TH1F *h_rebinned = new TH1F (h->GetName(),h->GetName(),nbins-1,bins);
    for(int i=0;i<h->GetXaxis()->GetNbins();i++){
        h_rebinned->SetBinContent(i+1,h->GetBinContent(i+1)/(bins[i+1]-bins[i]));
        h_rebinned->SetBinError(i+1,h->GetBinError(i+1)/(bins[i+1]-bins[i]));
    }
    return h_rebinned;
}

TH1F * RebinHistogramFixedBins(TH1F* h, bool saveEvents){ 
    float xmin = h->GetXaxis()->GetXmin();
    float xmax = h->GetXaxis()->GetXmax();
    int   nbin = h->GetXaxis()->GetNbins();

    TH1F *h_rebinned = new TH1F (h->GetName(),h->GetName(),nbin,xmin,xmax);
    for(int i=0;i<h->GetXaxis()->GetNbins();i++){
        if(!saveEvents){
            h_rebinned->SetBinContent(i+1,h->GetBinContent(i+1)/h->GetBinWidth(i+1));
            h_rebinned->SetBinError(i+1,h->GetBinError(i+1)/h->GetBinWidth(i+1));
        }else{
            h_rebinned->SetBinContent(i+1,h->GetBinContent(i+1));
            h_rebinned->SetBinError(i+1,h->GetBinError(i+1));
        }
    }
    
    return h_rebinned;
}


int main(int argc, char** argv)
{
    //##############################################
    //########    GLOBAL INITIALIZATION     ########
    //##############################################
    //load framework libraries
    gSystem->Load("libFWCoreFWLite");
    FWLiteEnabler::enable();

    //AutoLibraryLoader::enable();

    // initialize command line parser
    CommandLineParser parser ("Analyze FWLite Parameters");
    //parser.parseArguments(argc,argv);

    parser.addOption("inputFile",CommandLineParser::kString);
    parser.addOption("sample",CommandLineParser::kString);
    parser.addOption("xsection",CommandLineParser::kString);
    parser.addOption("nevents",CommandLineParser::kString);
    parser.addOption("label",CommandLineParser::kString);
    parser.addOption("channel",CommandLineParser::kString);
    parser.addOption("embedding",CommandLineParser::kString,"");
    parser.addOption("channelEB",CommandLineParser::kString,""); //this has to be tought how to propagate
    parser.addOption("ptcut1",CommandLineParser::kString);
    parser.addOption("etacut1",CommandLineParser::kString);
    parser.addOption("ptcut2",CommandLineParser::kString);
    parser.addOption("etacut2",CommandLineParser::kString);
    parser.addOption("met",CommandLineParser::kString);
    parser.addOption("dataOverMadgraphRW",CommandLineParser::kString);
    parser.addOption("nstart",CommandLineParser::kString,"Start loop","-1");
    parser.addOption("nstop",CommandLineParser::kString,"End loop","-1");

    parser.parseArguments(argc,argv);

    std::string inputFile_              = parser.stringValue("inputFile");
    std::string outputFile_             = parser.stringValue("outputFile");
    std::string xsection_               = parser.stringValue("xsection");
    std::string nevents_                = parser.stringValue("nevents");
    std::string sample_                 = parser.stringValue("sample");
    std::string label_                  = parser.stringValue("label");
    std::string channel_                = parser.stringValue("channel");
    std::string embedding_              = parser.stringValue("embedding"); 
    std::string channelForEmbedding_    = parser.stringValue("channelEB"); //this has to be tought how to propagate
    std::string ptcut1_                 = parser.stringValue("ptcut1");
    std::string etacut1_                = parser.stringValue("etacut1");
    std::string ptcut2_                 = parser.stringValue("ptcut2");
    std::string etacut2_                = parser.stringValue("etacut2");
    std::string met_                    = parser.stringValue("met");
    std::string dataOverMadgraphRW_     = parser.stringValue("dataOverMadgraphRW");
    std::string nstart_                 = parser.stringValue("nstart");
    std::string nstop_                  = parser.stringValue("nstop");

    //standard command: inputFile outputFile sample label channel channelEB ptcut1 etacut1 ptcut2 etacut2
    bool  isMM            = (channel_=="mm")?true:false;
    bool  isMT            = (channel_=="mt")?true:false;
    bool  embe            = (embedding_=="yes")?true:false;
    bool  isem            = false; if(embe){ isem = (channelForEmbedding_=="em")?true:false;}
    bool  iset            = false; if(embe){ iset = (channelForEmbedding_=="et")?true:false;}
    bool  ismt            = false; if(embe){ ismt = (channelForEmbedding_=="mt")?true:false;}
    bool  istt            = false; if(embe){ istt = (channelForEmbedding_=="tt")?true:false;}
    float xs_             = atof(xsection_.c_str());
    float nevt_           = atof(nevents_.c_str());
    float pt1_            = atof(ptcut1_.c_str());
    float pt2_            = atof(ptcut2_.c_str());
    float eta1_           = atof(etacut1_.c_str());
    float eta2_           = atof(etacut2_.c_str());
    bool  MET_            = (met_=="yes")?true:false; 
    bool  computeWeights_ = (dataOverMadgraphRW_=="yes")?true:false;
    int   nstart          = atoi(nstart_.c_str());
    int   nstop           = atoi(nstop_.c_str());
    bool  isMC            = (sample_!="SingleMuon")?true:false;

    //setup for systematics
    std::vector<string> varNames(1,"");
    if(isMM){
        varNames.push_back("_MESUp");      varNames.push_back("_MESDown");
        varNames.push_back("_MRESUp");     varNames.push_back("_MRESDown");
        varNames.push_back("_JECUp");      varNames.push_back("_JECDown");
        varNames.push_back("_bTagUp");     varNames.push_back("_bTagDown");
        varNames.push_back("_RWeight1Up"); varNames.push_back("_RWeight1Down");
        varNames.push_back("_RWeight2Up"); varNames.push_back("_RWeight2Down");
    }
    if(isMT){
        varNames.push_back("_MESUp");      varNames.push_back("_MESDown");
        varNames.push_back("_TESUp");      varNames.push_back("_TESDown");
        varNames.push_back("_JECUp");      varNames.push_back("_JECDown");
        varNames.push_back("_bTagUp");     varNames.push_back("_bTagDown");
        varNames.push_back("_RWeight1Up"); varNames.push_back("_RWeight1Down");
        varNames.push_back("_RWeight2Up"); varNames.push_back("_RWeight2Down");
    }


    //TFile *f_Double = TFile::Open((inputFile_).c_str());
    TFile *f_Double = TFile::Open(("root://eoscms.cern.ch//eos/cms//"+inputFile_).c_str());
    TTree *tree    = (TTree*) f_Double->Get("RLE_tree");

    MainTreeHandler *t=new MainTreeHandler(tree,channel_,isMC);;

    //initialize and retrieve all the corrections
    //data/MadGraph corrections

    //#### NEW FLOATING BINNING
    float ptBins_[]      = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 22, 24, 26, 28, 30, 32, 35, 38, 42, 46, 51, 57, 65, 77, 97, 1000};
    float massBins_[]    = {0, 51, 61, 71, 78, 82, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 97, 101, 1000};
    float etaBins_[]     = {-10, -4.3, -3.8, -3.4, -3.1, -2.9, -2.7, -2.5, -2.3, -2.1, -1.9, -1.7, -1.5, -1.3, -1.1, -0.9, -0.6, -0.3, 0.0, 0.3, 0.6, 0.9, 1.1, 
        1.3, 1.5, 1.7, 1.9, 2.1, 2.3, 2.5, 2.7, 2.9, 3.1, 3.4, 3.7, 4.1, 10};

    int nPtBins   = sizeof(ptBins_)/sizeof(*ptBins_);
    int nMassBins = sizeof(massBins_)/sizeof(*massBins_);
    int nEtaBins  = sizeof(etaBins_)/sizeof(*etaBins_);

    //***** NOT ANYMORE USEFUL
    /*TH1D *Zmass = NULL; TH1D *Zpt =  NULL; TH1D *Zeta  = NULL;
      Zmass  = new TH1D("Zmass", "Zmass", 1000,0,1000);
      Zpt    = new TH1D("Zpt"  , "Zpt"  , 1000,0,1000);
      Zeta   = new TH1D("Zeta" , "Zeta" , 200,-10,10);
      TH3D *reweighting_3d_noMET_fixed_bins    = NULL; 
      TH3D *reweighting_3d_MET_fixed_bins      = NULL; 
      TH3D *reweighting_3d_EM_noMET_fixed_bins = NULL; 
      TH3D *reweighting_3d_ET_noMET_fixed_bins = NULL; 
      TH3D *reweighting_3d_MT_noMET_fixed_bins = NULL; 
      TH3D *reweighting_3d_TT_noMET_fixed_bins = NULL;
      TH3D *reweighting_3d_MET                 = NULL; 
      TH3D *reweighting_3d_EM_MET              = NULL; 
      TH3D *reweighting_3d_ET_MET              = NULL; 
      TH3D *reweighting_3d_MT_MET              = NULL; 
      TH3D *reweighting_3d_TT_MET              = NULL;
      TH3D *reweighting_3d_EM_MET_fixed_bins   = NULL; 
      TH3D *reweighting_3d_ET_MET_fixed_bins   = NULL; 
      TH3D *reweighting_3d_MT_MET_fixed_bins   = NULL; 
      TH3D *reweighting_3d_TT_MET_fixed_bins   = NULL;
      reweighting_3d_MET                 = new TH3D ("NLOreweight_3d_MET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
      reweighting_3d_noMET_fixed_bins    = new TH3D ("NLOreweight_3d_noMET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_MET_fixed_bins      = new TH3D ("NLOreweight_3d_MET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_EM_noMET_fixed_bins = new TH3D ("NLOreweight_3d_EM_noMET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_ET_noMET_fixed_bins = new TH3D ("NLOreweight_3d_ET_noMET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_MT_noMET_fixed_bins = new TH3D ("NLOreweight_3d_MT_noMET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_TT_noMET_fixed_bins = new TH3D ("NLOreweight_3d_TT_noMET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_EM_MET              = new TH3D ("NLOreweight_3d_EM_MET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
      reweighting_3d_ET_MET              = new TH3D ("NLOreweight_3d_ET_MET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
      reweighting_3d_MT_MET              = new TH3D ("NLOreweight_3d_MT_MET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
      reweighting_3d_TT_MET              = new TH3D ("NLOreweight_3d_TT_MET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
      reweighting_3d_EM_MET_fixed_bins   = new TH3D ("NLOreweight_3d_EM_MET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_ET_MET_fixed_bins   = new TH3D ("NLOreweight_3d_ET_MET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_MT_MET_fixed_bins   = new TH3D ("NLOreweight_3d_MT_MET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      reweighting_3d_TT_MET_fixed_bins   = new TH3D ("NLOreweight_3d_TT_MET_fixed_bins","NLOreweight",50,0,500,60,0,300,50,-3,3);
      */
    TH3D *reweighting_3d_noMET               = NULL; 
    TH3D *reweighting_3d_EM_noMET            = NULL; 
    TH3D *reweighting_3d_ET_noMET            = NULL; 
    TH3D *reweighting_3d_MT_noMET            = NULL; 
    TH3D *reweighting_3d_TT_noMET            = NULL;

    TH1F *hEmbWeight_OS_Iso     = new TH1F("hEmbWeight_OS_Iso","hEmbWeight_OS_Iso",100,0,1);
    TH1F *hEmbWeight_OS_AntiIso = new TH1F("hEmbWeight_OS_AntiIso","hEmbWeight_OS_AntiIso",100,0,1);
    TH1F *hEmbWeight_SS_Iso     = new TH1F("hEmbWeight_SS_Iso","hEmbWeight_SS_Iso",100,0,1);
    TH1F *hEmbWeight_SS_AntiIso = new TH1F("hEmbWeight_SS_AntiIso","hEmbWeight_SS_AntiIso",100,0,1);

    TH3D *hrwos0_noMET                    = NULL;

    if(computeWeights_){
        reweighting_3d_noMET               = new TH3D ("NLOreweight_3d_noMET"   ,"NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
        reweighting_3d_EM_noMET            = new TH3D ("NLOreweight_3d_EM_noMET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
        reweighting_3d_ET_noMET            = new TH3D ("NLOreweight_3d_ET_noMET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
        reweighting_3d_MT_noMET            = new TH3D ("NLOreweight_3d_MT_noMET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);
        reweighting_3d_TT_noMET            = new TH3D ("NLOreweight_3d_TT_noMET","NLOreweight",nPtBins-1,ptBins_,nMassBins-1,massBins_,nEtaBins-1,etaBins_);

    }else{
        TFile *rw = new TFile((string(std::getenv("CMSSW_BASE"))+"/src/UserCode/ztautau_fwk/data/amcatnlo/reweighting.root").c_str());
        if(isMM && embe){
            if(isem)      hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_EM_noMET");
            else if(iset) hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_ET_noMET");
            else if(ismt) hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_MT_noMET");
            else if(istt) hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_TT_noMET");
        }else if(isMM && !embe){
            if(eta1_==(float)2.4 && pt1_==(float)13.0 && eta2_==(float)2.4 && pt2_==(float)10.0) hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_EM_noMET");
            if(eta1_==(float)2.1 && pt1_==(float)24.0 && eta2_==(float)2.3 && pt2_==(float)20.0) hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_ET_noMET"); 
            if(eta1_==(float)2.1 && pt1_==(float)19.0 && eta2_==(float)2.3 && pt2_==(float)20.0) hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_MT_noMET"); 
            if(eta1_==(float)2.1 && pt1_==(float)40.0 && eta2_==(float)2.1 && pt2_==(float)40.0) hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_TT_noMET"); 
        }else if(isMT && !embe){
            hrwos0_noMET = (TH3D*)rw->Get("rweight_3d_MT_noMET"); 
        }
    }

    bTagSF *bTagEfficiency = new bTagSF((string(std::getenv("CMSSW_BASE"))+"/src/UserCode/ztautau_fwk/data/btagweights/CSVv2.csv").c_str());
    if(isMM) bTagEfficiency->init_efficiency((string(std::getenv("CMSSW_BASE"))+"/src/UserCode/ztautau_fwk/data/btagweights/mm_bTagReweighiting.root").c_str());
    if(isMT) bTagEfficiency->init_efficiency((string(std::getenv("CMSSW_BASE"))+"/src/UserCode/ztautau_fwk/data/btagweights/mt_bTagReweighiting.root").c_str());

    ScaleFactor * muSFIdIso   = new ScaleFactor();
    ScaleFactor * muSFTrigger = new ScaleFactor();
    muSFIdIso->init_ScaleFactor((string(std::getenv("CMSSW_BASE"))+"/src/HTT-utilities/LepEffInterface/data/Muon/Muon_IdIso0p10_eff.root").c_str());
    muSFTrigger->init_ScaleFactor((string(std::getenv("CMSSW_BASE"))+"/src/HTT-utilities/LepEffInterface/data/Muon/Muon_SingleMu_eff.root").c_str());

    std::vector<string> stringCat;
    stringCat.push_back("inclusive");
    stringCat.push_back("0jet");
    stringCat.push_back("1jet_zpt_loose");
    stringCat.push_back("1jet_zpt_medium");
    stringCat.push_back("1jet_zpt_tight");
    stringCat.push_back("2jet_cp");
    stringCat.push_back("vbf");
    stringCat.push_back("1bjet");
    stringCat.push_back("2bjet");
    stringCat.push_back("2bjet_loose");
    stringCat.push_back("MSSM_btag");
    stringCat.push_back("1jet");
    stringCat.push_back("2jets");

    std::vector<string>stringIsolation;
    stringIsolation.push_back("");
    stringIsolation.push_back("_antiIso");

    std::vector<string>stringDataMG;
    stringDataMG.push_back("");
    stringDataMG.push_back("_DataMG");
    
    std::vector<string>stringEmbedding;
    stringEmbedding.push_back("");
    stringEmbedding.push_back("_DataMG");//_nominal");
    stringEmbedding.push_back("_DataMG_wTauSpinner");

    size_t ncatToInclude  = 1;  if(!computeWeights_) ncatToInclude  = stringCat.size();
    size_t nvarsToInclude = 1;  if(!computeWeights_) nvarsToInclude = varNames.size();
    size_t nisoToInclude  = 1;  if(!computeWeights_) nisoToInclude  = stringIsolation.size();
    size_t nmceToInclude  = 1;  if(!computeWeights_ && embe) nmceToInclude  = stringEmbedding.size();
    size_t nsfToInclude   = 1;  if(!computeWeights_ && !embe) nsfToInclude  = stringDataMG.size();
    
    std::vector<TDirectory*> foutDir;
    TFile *fout = TFile::Open(outputFile_.c_str(), "RECREATE");
    if(!computeWeights_){
        for(size_t ca=0; ca<ncatToInclude; ca++){
            TDirectory *cat =fout->mkdir((channel_+"_"+stringCat.at(ca)).c_str());
            foutDir.push_back(cat);
        }
    }

    TH1F* mvis_vh1[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* mvis_vh2[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* mvis_vh3[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* mvis_vh4[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* pvis_vh1[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* pvis_vh2[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* pvis_vh3[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* pvis_vh4[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* evis_vh1[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* evis_vh2[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* evis_vh3[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];
    TH1F* evis_vh4[ncatToInclude][nvarsToInclude][nisoToInclude][nmceToInclude][nsfToInclude];

    if(!computeWeights_){

        for(size_t ivar=0;ivar<nvarsToInclude;ivar++){
            for(size_t i=0;i<ncatToInclude;i++){
                for(size_t iiso=0;iiso<nisoToInclude; iiso++){
                    for(size_t imce=0;imce<nmceToInclude; imce++){
                        for(size_t isf=0;isf<nsfToInclude; isf++){
                            if(isMT){

                                TH1F *tmp_m = new TH1F (("h4"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),("h4"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),60,0,300);
                                TH1F *tmp_p = new TH1F (("h5"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),("h5"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),50,0,500);
                                TH1F *tmp_e = new TH1F (("h6"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),("h6"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),50,-3,3);

                                tmp_m->Sumw2();
                                tmp_p->Sumw2();
                                tmp_e->Sumw2();

                                mvis_vh1[i][ivar][iiso][imce][isf]=(TH1F*)tmp_m->Clone();
                                mvis_vh2[i][ivar][iiso][imce][isf]=(TH1F*)tmp_m->Clone();
                                mvis_vh3[i][ivar][iiso][imce][isf]=(TH1F*)tmp_m->Clone();
                                mvis_vh4[i][ivar][iiso][imce][isf]=(TH1F*)tmp_m->Clone();
                                pvis_vh1[i][ivar][iiso][imce][isf]=(TH1F*)tmp_p->Clone();
                                pvis_vh2[i][ivar][iiso][imce][isf]=(TH1F*)tmp_p->Clone();
                                pvis_vh3[i][ivar][iiso][imce][isf]=(TH1F*)tmp_p->Clone();
                                pvis_vh4[i][ivar][iiso][imce][isf]=(TH1F*)tmp_p->Clone();
                                evis_vh1[i][ivar][iiso][imce][isf]=(TH1F*)tmp_e->Clone();
                                evis_vh2[i][ivar][iiso][imce][isf]=(TH1F*)tmp_e->Clone();
                                evis_vh3[i][ivar][iiso][imce][isf]=(TH1F*)tmp_e->Clone();
                                evis_vh4[i][ivar][iiso][imce][isf]=(TH1F*)tmp_e->Clone();
                            }
                            if(isMM){
                                TH1F *tmp_m = new TH1F (("h4"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),("h4"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),60,0,300);
                                TH1F *tmp_p = new TH1F (("h5"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),("h5"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),50,0,500);
                                TH1F *tmp_e = new TH1F (("h6"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),("h6"+stringCat[i]+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]+stringDataMG[isf]).c_str(),50,-3,3);

                                tmp_m->Sumw2();
                                tmp_p->Sumw2();
                                tmp_e->Sumw2();

                                mvis_vh1[i][ivar][iiso][imce][isf]=(TH1F*)tmp_m->Clone();
                                mvis_vh2[i][ivar][iiso][imce][isf]=(TH1F*)tmp_m->Clone();
                                pvis_vh1[i][ivar][iiso][imce][isf]=(TH1F*)tmp_p->Clone();
                                pvis_vh2[i][ivar][iiso][imce][isf]=(TH1F*)tmp_p->Clone();
                                evis_vh1[i][ivar][iiso][imce][isf]=(TH1F*)tmp_e->Clone();
                                evis_vh2[i][ivar][iiso][imce][isf]=(TH1F*)tmp_e->Clone();
                            }
                        }
                    }
                }
            }
        }
    }

    if(nstart==-1 && nstop==-1){ nstart=0; nstop=(Int_t) tree->GetEntries();}

    float poids=1.0; 
    float luminosity=2262.144;

    std::vector<float> btag_weight;
    std::vector<float> final_weight;
    std::vector<bool>  categories;

    bool   print     = false;
    double sqrtS     = 13.e+3;
    int    verbosity = 0;

    bool   selection;
    double lepSF;
    double fweight;
    double mgweight0;
    double cand1SF1, cand1SF2;
    double cand2SF1, cand2SF2;

    int decayMode1 = -999;
    int decayMode2 = -999;
    if(embe && isem){ decayMode1=MCEmbeddingWeights::kTauToElecDecay; decayMode2=MCEmbeddingWeights::kTauToMuDecay;}
    if(embe && iset){ decayMode1=MCEmbeddingWeights::kTauToElecDecay; decayMode2=MCEmbeddingWeights::kTauToHadDecay;}
    if(embe && ismt){ decayMode1=MCEmbeddingWeights::kTauToMuDecay;   decayMode2=MCEmbeddingWeights::kTauToHadDecay;}
    if(embe && istt){ decayMode1=MCEmbeddingWeights::kTauToHadDecay;  decayMode2=MCEmbeddingWeights::kTauToHadDecay;}
    MCEmbeddingWeights mcEmbeddingWeights(decayMode1, pt1_, eta1_, decayMode2, pt2_, eta2_, sqrtS,verbosity);

    TBenchmark* clock = new TBenchmark();
    clock->Start("<runZtautauAnalysis>");

    for (Int_t i = nstart; i < nstop; i++) {

        tree->GetEntry(i);
       
        if (i % 20000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nstop);
        fflush(stdout);

        if(print) cout << "EVENT/RUN/LUMI " << t->evt << "/" << t->run << "/" << t->lumi << endl; 
        if(print) cout << "t->ptype_1/t->ptype_2 " << t->ptype_1 << "/" << t->ptype_2 << endl;
        if(print) cout << " isMM " << isMM << " isMT " << isMT << endl;
         
        if(isMM){if(t->ptype_2!=0) continue;}
        if(isMT){if(t->ptype_2!=2) continue;} 
        
        if ((label_=="ZTT") && t->gen_match_2!=5) continue;
        if ((label_=="ZLL") && (t->gen_match_2!=6 && t->gen_match_2>4)) continue;
       
        if(isMC){
            if((sample_=="DY1Jets") || (sample_=="DY2Jets") || (sample_=="DY3Jets") || (sample_=="DY4Jets") || (sample_.find("DYJets_")!=std::string::npos)){
                if(t->lheNOutPartons==0) poids = 0.0560117405; 
                if(t->lheNOutPartons==1) poids = 0.023608783821; 
                if(t->lheNOutPartons==2) poids = 0.0245697358;
                if(t->lheNOutPartons==3) poids = 0.025551557068; 
                if(t->lheNOutPartons==4) poids = 0.021317352373; 
            }else if(sample_=="W1Jets" || sample_=="W2Jets" || sample_=="W3Jets" || sample_=="W4Jets" || sample_=="WJets"){
                if(t->lheNOutPartons==0) poids = 2.95238926778284;
                if(t->lheNOutPartons==1) poids = 0.490734250648282;
                if(t->lheNOutPartons==2) poids = 0.267667463342625;
                if(t->lheNOutPartons==3) poids = 0.13737662529981;
                if(t->lheNOutPartons==4) poids = 0.147002677407227;
            }else{
                poids = luminosity*xs_/nevt_;
            }
        }

        TLorentzVector cand1, cand2;

        int icand=-999;

        if((isMM && !embe) || (isMM && embe)){
            srand (time(NULL));
            icand = rand() % 2 + 1;
            if(icand==1){
                cand1.SetPtEtaPhiM(t->pt_1,t->eta_1,t->phi_1,t->m_1);
                cand2.SetPtEtaPhiM(t->pt_2,t->eta_2,t->phi_2,t->m_2);
            }else if(icand==2){
                cand1.SetPtEtaPhiM(t->pt_2,t->eta_2,t->phi_2,t->m_2);
                cand2.SetPtEtaPhiM(t->pt_1,t->eta_1,t->phi_1,t->m_1);
            }
        }else if(isMT){
            cand1.SetPtEtaPhiM(t->pt_1,t->eta_1,t->phi_1,t->m_1);
            cand2.SetPtEtaPhiM(t->pt_2,t->eta_2,t->phi_2,t->m_2);
        }


        for(size_t ivar=0; ivar<nvarsToInclude; ivar++){

            selection=false;
            fweight=1;
            mgweight0=1;
            lepSF=1;
            cand1SF1=1; cand1SF2=1;
            cand2SF1=1; cand2SF2=1;
            btag_weight.clear();
            categories.clear();
            final_weight.clear();

            float mex=t->met_px; 
            float mey=t->met_py;

            bool varyMESUp       ( varNames[ivar]=="_MESUp"         );
            bool varyTESUp       ( varNames[ivar]=="_TESUp"         );
            bool varyJECUp       ( varNames[ivar]=="_JECUp"         );
            bool varyMRESUp      ( varNames[ivar]=="_MRESUp"         );
            bool varybTagUp      ( varNames[ivar]=="_bTagUp"         );
            bool varyRWeight1Up  ( varNames[ivar]=="_RWeight1Up"     );
            bool varyRWeight2Up  ( varNames[ivar]=="_RWeight2Up"     );
            bool varyMESDown     ( varNames[ivar]=="_MESDown"         );
            bool varyTESDown     ( varNames[ivar]=="_TESDown"         );
            bool varyJECDown     ( varNames[ivar]=="_JECDown"         );
            bool varyMRESDown    ( varNames[ivar]=="_MRESDown"         );
            bool varybTagDown    ( varNames[ivar]=="_bTagDown"         );
            bool varyRWeight1Down( varNames[ivar]=="_RWeight1Down"   );
            bool varyRWeight2Down( varNames[ivar]=="_RWeight2Down"   );

            float syst=0;

            if(varyTESUp){   cand2*=1.03; mex=mex-0.03*(cand2.Px()/1.03); mey=mey-0.03*(cand2.Py()/1.03); }
            if(varyTESDown){ cand2*=0.97; mex=mex+0.03*(cand2.Px()/0.97); mey=mey+0.03*(cand2.Py()/0.97); }
            if(varyMESUp){   cand1*=1.01; if(isMM) cand2*=1.01; }
            if(varyMESDown){ cand1*=0.99; if(isMM) cand2*=0.99; }
            if(varybTagUp)   syst=1;
            if(varybTagDown) syst=-1;


            TLorentzVector MET(mex,mey,0,0);
            TLorentzVector Z     = cand1+cand2;
            TLorentzVector ZMET  = cand1+cand2+MET;
            float mvis = Z.M();
            float mt=TMass_F(cand1.Pt(),MET.Pt(),cand1.Px(),MET.Px(),cand1.Py(),MET.Py());

            if(varyMRESUp)   mvis = 1.2 * (Z.M()  - 91.2) + 91.2;
            if(varyMRESDown) mvis = 0.8 * (Z.M()  - 91.2) + 91.2;

            if(isMM && !computeWeights_) selection = (abs(cand1.Eta())<eta1_ && cand1.Pt()>pt1_  && 
                    abs(cand2.Eta())<eta2_ && cand2.Pt()>pt2_ && !t->extraelec_veto && !t->extramuon_veto);// && mvis>60 && mvis<120);
            if(isMT && !computeWeights_) selection = (abs(cand1.Eta())<eta1_ && cand1.Pt()>pt1_  && abs(cand2.Eta())<eta2_ && cand2.Pt()>pt2_ &&
                    t->againstElectronVLooseMVA6_2 && t->decayModeFindingOldDMs_2 && t->againstMuonTight3_2 &&
                    !t->dilepton_veto && !t->extraelec_veto && !t->extramuon_veto);
            if(isMM && computeWeights_){ selection = (abs(cand1.Eta())<eta1_ && cand1.Pt()>pt1_  && 
                    abs(cand2.Eta())<eta2_ && cand2.Pt()>pt2_ && !t->extraelec_veto && !t->extramuon_veto && mvis>12);
            }
            if(!selection) continue;
           
            cand1SF1          = (muSFIdIso->get_ScaleFactor(cand1.Pt(),cand1.Eta()));
            cand1SF2          = (muSFTrigger->get_ScaleFactor(cand1.Pt(),cand1.Eta()));
            if(isMM) cand2SF1 = (muSFIdIso->get_ScaleFactor(cand2.Pt(),cand2.Eta()));
            lepSF             = cand1SF1*cand1SF2*cand2SF1*cand2SF2;

            if(isMC){
                fweight = poids*t->trigweight_1*t->trigweight_2*lepSF;
            }

            int nbJets=0; int nJets30=0;
            int b1_index=-1; int b2_index=-1; int j1_index=-1; int j2_index=-1;
            TLorentzVector jet1, jet2, bjet1, bjet2;
            
            for (uint ij = 0; ij < t->jets_px->size(); ij++){
                TLorentzVector jet(t->jets_px->at(ij),t->jets_py->at(ij),t->jets_pz->at(ij),t->jets_e->at(ij));
                if (varyJECUp)   jet*=(1+t->jets_jecUnc->at(ij)); //JEC SYS UP   propa->ation
                if (varyJECDown) jet*=(1-t->jets_jecUnc->at(ij)); //JEC SYS DOWN propa->ation
                if (t->PFjetID->at(ij)==0) continue;
                if (Overlap_3(cand1,cand2,jet)) continue;
                if (t->bCSVscore->at(ij)>0.460 && jet.Pt()>20 && fabs(jet.Eta()<2.4)){
                    nbJets++;
                    if (nbJets==1)      b1_index = ij;
                    else if (nbJets==2) b2_index = ij;

                }
                if (jet.Pt()>30 && fabs(jet.Eta()<4.7)){
                    nJets30++;
                    if (nJets30==1)      j1_index = ij;
                    else if (nJets30==2) j2_index = ij;
                    
                }
            }
            
            float jcsv_1       = -999;
            float jcsv_2       = -999;
            float bcsv_1       = -999;
            float bcsv_2       = -999;
            int   genjet_HF_1  = -999;
            int   genjet_HF_2  = -999;
            int   genbjet_HF_1 = -999;
            int   genbjet_HF_2 = -999;
            
            if(j1_index>-1){
                jet1.SetPxPyPzE(t->jets_px->at(j1_index),t->jets_py->at(j1_index),t->jets_pz->at(j1_index),t->jets_e->at(j1_index));
                jcsv_1 = t->bCSVscore->at(j1_index);
                if (isMC){
                    int genjet_index_1 = t->jets_genjetIndex->at(j1_index);
                    if(genjet_index_1>-1) genjet_HF_1=t->genjet_hadronFlavour->at(genjet_index_1);
                }
                if(nJets30>1){
                    jet2.SetPxPyPzE(t->jets_px->at(j2_index),t->jets_py->at(j2_index),t->jets_pz->at(j2_index),t->jets_e->at(j2_index));
                    jcsv_2 = t->bCSVscore->at(j2_index);
                    if (isMC){
                        int genjet_index_2 = t->jets_genjetIndex->at(j2_index);
                        if(genjet_index_2>-1) genjet_HF_2=t->genjet_hadronFlavour->at(genjet_index_2);
                    }
                }else jet2.SetPxPyPzE(-999,-999,-999,-999);
            }else{
                jet1.SetPxPyPzE(-999,-999,-999,-999);
                jet2.SetPxPyPzE(-999,-999,-999,-999);
            }
            
            if(print) cout << " debug 4 " << endl;

            if(nbJets>0){
                bjet1.SetPxPyPzE(t->jets_px->at(b1_index),t->jets_py->at(b1_index),t->jets_pz->at(b1_index),t->jets_e->at(b1_index));
                bcsv_1 = t->bCSVscore->at(b1_index);
                if (isMC){
                    int genbjet_index_1 = t->jets_genjetIndex->at(b1_index);
                    if(genbjet_index_1>-1) genbjet_HF_1=t->genjet_hadronFlavour->at(genbjet_index_1);
                }
                if(nbJets>1){
                    bjet2.SetPxPyPzE(t->jets_px->at(b2_index),t->jets_py->at(b2_index),t->jets_pz->at(b2_index),t->jets_e->at(b2_index));
                    bcsv_2 = t->bCSVscore->at(b2_index);
                    if (isMC){
                        int genbjet_index_2 = t->jets_genjetIndex->at(b2_index);
                        if(genbjet_index_2>-1) genbjet_HF_2=t->genjet_hadronFlavour->at(genbjet_index_2);
                    }
                }else bjet2.SetPxPyPzE(-999,-999,-999,-999);
            }else{
                bjet1.SetPxPyPzE(-999,-999,-999,-999);
                bjet2.SetPxPyPzE(-999,-999,-999,-999);
            }
            
            if(print) cout << " debug 5 " << endl;
            

            TLorentzVector dijet = jet1+jet2;
            float mjj   = -999;
            float jdeta = -999;
            
            if(jet1.Phi()!=-999 && jet2.Phi()!=-999){
                jdeta = fabs(jet1.Eta()-jet2.Eta());
                mjj   = dijet.M();
            }

            int njetingap=0; 
            if (j1_index<0 or j2_index<0) njetingap=-2;
            else{
                for (uint ij = 0; ij < t->jets_px->size(); ij++) {
                    TLorentzVector jet(t->jets_px->at(ij),t->jets_py->at(ij),t->jets_pz->at(ij),t->jets_e->at(ij));
                    if (varyJECUp)   jet*=(1+t->jets_jecUnc->at(ij)); //JEC SYS UP   propagation
                    if (varyJECDown) jet*=(1-t->jets_jecUnc->at(ij)); //JEC SYS DOWN propagation
                    if (jet.Pt()<30 or fabs(jet.Eta())>4.7 or t->PFjetID->at(ij)==0) continue;
                    if (Overlap_3(cand1,cand2,jet)) continue;
                    if (Overlap_3(jet1,jet2,jet)) continue;
                    if ((jet.Eta()>jet1.Eta() && jet.Eta()<jet2.Eta()) or (jet.Eta()>jet2.Eta() && jet.Eta()<jet1.Eta())) njetingap++;
                }
            }
            
            if(print) cout << " jet selection " << endl;

            categories.push_back(true);
            float ptz;
            if(MET_) ptz = ZMET.Pt();
            else     ptz = Z.Pt();   

            if(!computeWeights_){
                if(nJets30==0) categories.push_back(true); else categories.push_back(false);
                if(nJets30==1 && ptz<50) categories.push_back(true); else categories.push_back(false);
                if(nJets30==1 && ptz>50 && ptz<100) categories.push_back(true); else categories.push_back(false);
                if(nJets30==1 && ptz>100) categories.push_back(true); else categories.push_back(false);
                if(nJets30==2 && mjj>200 && dR(jet1.Eta(),jet1.Phi(),jet2.Eta(),jet2.Phi())>0.6) categories.push_back(true); else categories.push_back(false);
                if(nJets30==2 && mjj>500 && jdeta>3.5 && njetingap==0) categories.push_back(true); else categories.push_back(false);
                if(nbJets==1 && (bcsv_1>0.800 || bcsv_2>0.800)) categories.push_back(true); else categories.push_back(false);
                if(nbJets==2 && bcsv_1>0.800 && bcsv_2>0.800) categories.push_back(true); else categories.push_back(false);
                if(nbJets==2 && bcsv_1>0.460 && bcsv_2>0.460) categories.push_back(true); else categories.push_back(false);
            }


            bool jets_coincidence_same_idx = false;
            bool jets_coincidence_swap_idx = false;
            bool jet1_coincidence_same_idx = false;
            bool jet1_coincidence_swap_idx = false;
            bool jet2_coincidence_same_idx = false;
            bool jet2_coincidence_swap_idx = false;
    
            std::vector<TLorentzVector> jets;
            std::vector<float> jets_csv;
            std::vector<int>   jets_HF;
            int nbjetM_below30 = 0; 
            int nbjetM_above30 = 0;
            int njets = 0;


            if((jcsv_1==bcsv_1) && (jcsv_2==bcsv_2))      {jets_coincidence_same_idx = true; } //cout << "j1=b1 and j2=b2"<< endl; } //b1,b2
            else if((jcsv_1==bcsv_2) && (jcsv_2==bcsv_1)) {jets_coincidence_swap_idx = true; } //cout << "j1=b2 and j2=b1"<< endl; }//b1,b2
            else if((jcsv_1==bcsv_1) && (jcsv_2!=bcsv_2)) {jet1_coincidence_same_idx = true; } //cout << "j1=b1 and j2!=b2"<<endl; }//b1,b2,j2
            else if((jcsv_1==bcsv_2) && (jcsv_2!=bcsv_1)) {jet1_coincidence_swap_idx = true; } //cout << "j1=b2 and j2!=b1"<<endl; }//b1,b2,j2
            else if((jcsv_1!=bcsv_1) && (jcsv_2==bcsv_2)) {jet2_coincidence_same_idx = true; } //cout << "j1!=b1 and j2=b2"<<endl; }//b1,b2,j1
            else if((jcsv_1!=bcsv_2) && (jcsv_2==bcsv_1)) {jet2_coincidence_swap_idx = true; } //cout << "j1!=b2 and j2==b1"<<endl; } //b1,b2,j1

            if(bcsv_1>0.800 && bjet1.Pt()<30) nbjetM_below30++;
            if(bcsv_2>0.800 && bjet2.Pt()<30) nbjetM_below30++;
            if(bcsv_1>0.800 && bjet1.Pt()>30) nbjetM_above30++;
            if(bcsv_2>0.800 && bjet2.Pt()>30) nbjetM_above30++;

            if(jets_coincidence_same_idx || jets_coincidence_swap_idx){
                if(bjet1.Px()!=-999) {
                    jets.push_back(bjet1); jets_csv.push_back(bcsv_1); jets_HF.push_back(genbjet_HF_1);}
                if(bjet2.Px()!=-999) {
                    jets.push_back(bjet2); jets_csv.push_back(bcsv_2); jets_HF.push_back(genbjet_HF_2);}
            }else if(jet1_coincidence_same_idx || jet1_coincidence_swap_idx){
                if(jet2.Px()!=-999) {
                    jets.push_back(jet2);  jets_csv.push_back(jcsv_2); jets_HF.push_back(genjet_HF_2);}
                if(bjet1.Px()!=-999) {
                    jets.push_back(bjet1); jets_csv.push_back(bcsv_1); jets_HF.push_back(genbjet_HF_1);}
                if(bjet2.Px()!=-999) {
                    jets.push_back(bjet2); jets_csv.push_back(bcsv_2); jets_HF.push_back(genbjet_HF_2);}
                if(jet2.Pt()>30 && jet2.Px()!=-999) njets++;
            }else if(jet2_coincidence_same_idx || jet2_coincidence_swap_idx){
                if(jet1.Px()!=-999) { jets.push_back(jet1);  jets_csv.push_back(jcsv_1); jets_HF.push_back(genjet_HF_1); }
                if(bjet1.Px()!=-999){ jets.push_back(bjet1); jets_csv.push_back(bcsv_1); jets_HF.push_back(genbjet_HF_1);}
                if(bjet2.Px()!=-999){ jets.push_back(bjet2); jets_csv.push_back(bcsv_2); jets_HF.push_back(genbjet_HF_2);}
                if(jet1.Pt()>30 && jet1.Px()!=-999) njets++;
            }else{
                if(jet1.Px()!=-999) { jets.push_back(jet1);  jets_csv.push_back(jcsv_1); jets_HF.push_back(genjet_HF_1); }
                if(jet2.Px()!=-999) { jets.push_back(jet2);  jets_csv.push_back(jcsv_2); jets_HF.push_back(genjet_HF_2);}
                if(bjet1.Px()!=-999){ jets.push_back(bjet1); jets_csv.push_back(bcsv_1); jets_HF.push_back(genbjet_HF_1);}
                if(bjet2.Px()!=-999){ jets.push_back(bjet2); jets_csv.push_back(bcsv_2); jets_HF.push_back(genbjet_HF_2);}
                if(jet1.Pt()>30 && jet1.Px()!=-999) njets++;
                if(jet2.Pt()>30 && jet2.Px()!=-999) njets++;
            }

 
            if(!computeWeights_){
                if( ((nbjetM_above30 + nbjetM_below30) >=1) && ((njets+nbjetM_above30)<=1) ) categories.push_back(true); else categories.push_back(false);
                //if((nbjetM_above30>=1 && njets==0) || (nbjetM_below30>=1 && njets<=1)) categories.push_back(true); else categories.push_back(false);
                if(nJets30==1) categories.push_back(true); else categories.push_back(false);
                if(nJets30>=2) categories.push_back(true); else categories.push_back(false);
            }

            btag_weight = bTagEfficiency->getEvtWeight(jets_csv,jets,jets_HF,syst);
            if(isMC) fweight = fweight*btag_weight.at(1);

            //--> weights from the look-up table extracted by using the gen level 4 vector mother information
            if(!computeWeights_ && ((isMT && label_=="ZTT") || (isMM && label_=="ZLL"))){
                int bin0  = hrwos0_noMET->FindBin(t->gen_Z_pt,t->gen_Z_mass,t->gen_Z_eta);
                mgweight0 = hrwos0_noMET->GetBinContent(bin0);
            }

            if(varyRWeight1Up)   {  mgweight0 = 1.0 + 1.7*(mgweight0-1);  } 
            if(varyRWeight1Down) {  mgweight0 = 1.0 + 0.3*(mgweight0-1);  } 
            if(varyRWeight2Up)   {  mgweight0 = 1.0 + 1.7*(mgweight0-1);  } 
            if(varyRWeight2Down) {  mgweight0 = 1.0 + 0.3*(mgweight0-1);  } 

            final_weight.push_back(fweight);
            final_weight.push_back(fweight*mgweight0);

            if(embe && !computeWeights_){
                double evtWeight_wTauSpinner  = -999;
                if(icand==1){
                    reco::Particle::LorentzVector muon1P4(t->px_1, t->py_1, t->pz_1, t->e_1);
                    reco::Particle::LorentzVector muon2P4(t->px_2, t->py_2, t->pz_2, t->e_2);
                    evtWeight_wTauSpinner  = mcEmbeddingWeights(muon1P4, t->q_1, muon2P4, t->q_2, true);
                }else if(icand==2){
                    reco::Particle::LorentzVector muon1P4(t->px_2, t->py_2, t->pz_2, t->e_2);
                    reco::Particle::LorentzVector muon2P4(t->px_1, t->py_1, t->pz_1, t->e_1);
                    evtWeight_wTauSpinner  = mcEmbeddingWeights(muon1P4, t->q_2, muon2P4, t->q_1, true);
                }                                                             
                final_weight.push_back(fweight*mgweight0*evtWeight_wTauSpinner);  
            }
            
            if (!computeWeights_){            
                for(size_t ca=0; ca<ncatToInclude; ca++){
                    if(categories.at(ca)){
                        for(size_t isf=0;isf<nsfToInclude;isf++){ 
                            if(isMT){
                                if(t->q_1*t->q_2<0 && mt<30){
                                    if(t->iso_1<0.1 && t->byTightIsolationMVArun2v1DBoldDMwLT_2){ 
                                        mvis_vh1[ca][ivar][0][0][isf]->Fill(mvis,final_weight.at(isf));
                                        pvis_vh1[ca][ivar][0][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                        evis_vh1[ca][ivar][0][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                    }
                                    if(t->iso_1>0.1 && t->iso_1<0.5){
                                        mvis_vh1[ca][ivar][1][0][isf]->Fill(mvis,final_weight.at(isf));
                                        pvis_vh1[ca][ivar][1][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                        evis_vh1[ca][ivar][1][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                    }
                                } //closed OS and low mt
                                if(t->q_1*t->q_2>0 && mt<30){
                                    if(t->iso_1<0.1 && t->byTightIsolationMVArun2v1DBoldDMwLT_2){ 
                                        mvis_vh2[ca][ivar][0][0][isf]->Fill(mvis,final_weight.at(isf));
                                        pvis_vh2[ca][ivar][0][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                        evis_vh2[ca][ivar][0][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                    }
                                    if(t->iso_1>0.1 && t->iso_1<0.5){
                                        mvis_vh2[ca][ivar][1][0][isf]->Fill(mvis,final_weight.at(isf));
                                        pvis_vh2[ca][ivar][1][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                        evis_vh2[ca][ivar][1][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                    }
                                } //closed SS and low mt
                                if(t->q_1*t->q_2<0 && mt>70){
                                    if(t->iso_1<0.1 && t->byTightIsolationMVArun2v1DBoldDMwLT_2){ 
                                        mvis_vh3[ca][ivar][0][0][isf]->Fill(mvis,final_weight.at(isf));
                                        pvis_vh3[ca][ivar][0][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                        evis_vh3[ca][ivar][0][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                    }
                                    if(t->iso_1>0.1 && t->iso_1<0.5){
                                        mvis_vh3[ca][ivar][1][0][isf]->Fill(mvis,final_weight.at(isf));
                                        pvis_vh3[ca][ivar][1][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                        evis_vh3[ca][ivar][1][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                    }
                                } //closed OS and high mt
                                if(t->q_1*t->q_2>0 && mt>70){
                                    if(t->iso_1<0.1 && t->byTightIsolationMVArun2v1DBoldDMwLT_2){ 
                                        mvis_vh4[ca][ivar][0][0][isf]->Fill(mvis,final_weight.at(isf));
                                        pvis_vh4[ca][ivar][0][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                        evis_vh4[ca][ivar][0][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                    }
                                    if(t->iso_1>0.1 && t->iso_1<0.5){
                                        mvis_vh4[ca][ivar][1][0][isf]->Fill(mvis,final_weight.at(isf));
                                        pvis_vh4[ca][ivar][1][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                        evis_vh4[ca][ivar][1][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                    }
                                } //closed SS and high mt
                            } //closed is MT
                            if(isMM && !embe){
                                if(t->q_1*t->q_2<0 && t->iso_1<0.1 && t->iso_2<0.1){
                                    mvis_vh1[ca][ivar][0][0][isf]->Fill(mvis,final_weight.at(isf));
                                    pvis_vh1[ca][ivar][0][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                    evis_vh1[ca][ivar][0][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                } //closed OS & signal iso
                                if(t->q_1*t->q_2>0 && t->iso_1<0.1 && t->iso_2<0.1){
                                    mvis_vh2[ca][ivar][0][0][isf]->Fill(mvis,final_weight.at(isf));
                                    pvis_vh2[ca][ivar][0][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                    evis_vh2[ca][ivar][0][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                } //closed SS and signal iso
                                if(t->q_1*t->q_2<0 && t->iso_1>0.1 && t->iso_1<0.5 && t->iso_2>0.1 && t->iso_2<0.5){
                                    mvis_vh1[ca][ivar][1][0][isf]->Fill(mvis,final_weight.at(isf));
                                    pvis_vh1[ca][ivar][1][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                    evis_vh1[ca][ivar][1][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                } //closed OS and control iso
                                if(t->q_1*t->q_2>0 && t->iso_1>0.1 && t->iso_1<0.5 && t->iso_2>0.1 && t->iso_2<0.5){
                                    mvis_vh2[ca][ivar][1][0][isf]->Fill(mvis,final_weight.at(isf));
                                    pvis_vh2[ca][ivar][1][0][isf]->Fill(Z.Pt(),final_weight.at(isf));
                                    evis_vh2[ca][ivar][1][0][isf]->Fill(Z.Eta(),final_weight.at(isf));
                                } //closed SS and control iso
                            }//closed isMM
                        }//closed loop on data/MG SF
                        if(isMM && embe){
                            for(size_t imce=0;imce<nmceToInclude;imce++){

                                if(t->q_1*t->q_2<0 && t->iso_1<0.1 && t->iso_2<0.1){

                                    if(ca==0 && ivar==0 && imce==2) hEmbWeight_OS_Iso->Fill(final_weight.at(imce));

                                    mvis_vh1[ca][ivar][0][imce][0]->Fill(mvis,final_weight.at(imce)); 
                                    pvis_vh1[ca][ivar][0][imce][0]->Fill(Z.Pt(),final_weight.at(imce));
                                    evis_vh1[ca][ivar][0][imce][0]->Fill(Z.Eta(),final_weight.at(imce));
                                } //closed OS & signal iso
                                if(t->q_1*t->q_2>0 && t->iso_1<0.1 && t->iso_2<0.1){
                                    
                                    if(ca==0 && ivar==0 && imce==2) hEmbWeight_SS_Iso->Fill(final_weight.at(imce));
                                    mvis_vh2[ca][ivar][0][imce][0]->Fill(mvis,final_weight.at(imce));
                                    pvis_vh2[ca][ivar][0][imce][0]->Fill(Z.Pt(),final_weight.at(imce));
                                    evis_vh2[ca][ivar][0][imce][0]->Fill(Z.Eta(),final_weight.at(imce));
                                } //closed SS and signal iso
                                if(t->q_1*t->q_2<0 && t->iso_1>0.1 && t->iso_1<0.5 && t->iso_2>0.1 && t->iso_2<0.5){
                                    if(ca==0 && ivar==0 && imce==2) hEmbWeight_OS_AntiIso->Fill(final_weight.at(imce));
                                    mvis_vh1[ca][ivar][1][imce][0]->Fill(mvis,final_weight.at(imce));
                                    pvis_vh1[ca][ivar][1][imce][0]->Fill(Z.Pt(),final_weight.at(imce));
                                    evis_vh1[ca][ivar][1][imce][0]->Fill(Z.Eta(),final_weight.at(imce));
                                } //closed OS and control iso
                                if(t->q_1*t->q_2>0 && t->iso_1>0.1 && t->iso_1<0.5 && t->iso_2>0.1 && t->iso_2<0.5){
                                    if(ca==0 && ivar==0 && imce==2) hEmbWeight_SS_AntiIso->Fill(final_weight.at(imce));
                                    mvis_vh2[ca][ivar][1][imce][0]->Fill(mvis,final_weight.at(imce));
                                    pvis_vh2[ca][ivar][1][imce][0]->Fill(Z.Pt(),final_weight.at(imce));
                                    evis_vh2[ca][ivar][1][imce][0]->Fill(Z.Eta(),final_weight.at(imce));
                                } //closed SS and control iso
                            }//end loop for embedding MM
                        }//end if
                    }//end check of category bool
                }//end of category loop
            }//end loop on compute weight
            else{
                if (t->q_1*t->q_2<0 && t->iso_1<0.1 && t->iso_2<0.1 && isMM && !embe){
                    reweighting_3d_noMET->Fill(Z.Pt(),Z.M(),Z.Eta(),fweight);

                    if(abs(cand1.Eta())<2.4 && cand1.Pt()>13 && abs(cand2.Eta())<2.4 && cand2.Pt()>10){ 
                        reweighting_3d_EM_noMET->Fill(Z.Pt(),Z.M(),Z.Eta(),fweight);
                    }
                    if(abs(cand1.Eta())<2.1 && cand1.Pt()>24 && abs(cand2.Eta())<2.3 && cand2.Pt()>20){ 
                        reweighting_3d_ET_noMET->Fill(Z.Pt(),Z.M(),Z.Eta(),fweight);
                    }
                    if(abs(cand1.Eta())<2.1 && cand1.Pt()>19 && abs(cand2.Eta())<2.3 && cand2.Pt()>20){ 
                        reweighting_3d_MT_noMET->Fill(Z.Pt(),Z.M(),Z.Eta(),fweight);
                    }
                    if(abs(cand1.Eta())<2.1 && cand1.Pt()>40 && abs(cand2.Eta())<2.1 && cand2.Pt()>40){ 
                        reweighting_3d_TT_noMET->Fill(Z.Pt(),Z.M(),Z.Eta(),fweight);
                    }
                }
            }
        }//end of loop over systematics
    } // end of loop over events

    if(!computeWeights_){
        for(size_t ca=0; ca<ncatToInclude; ca++){
            fout->cd();
            foutDir.at(ca)->cd();
            for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
                for(size_t iiso=0; iiso<nisoToInclude; iiso++){
                    for(size_t isf=0; isf<nsfToInclude; isf++){
                        if(isMT){

                            //additional histograms for check
                            mvis_vh1[ca][ivar][iiso][0][isf]->SetName(("h_low_mt_OS_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            mvis_vh2[ca][ivar][iiso][0][isf]->SetName(("h_low_mt_SS_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            mvis_vh3[ca][ivar][iiso][0][isf]->SetName(("h_high_mt_OS_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            mvis_vh4[ca][ivar][iiso][0][isf]->SetName(("h_high_mt_SS_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            pvis_vh1[ca][ivar][iiso][0][isf]->SetName(("h_low_mt_OS_pt_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            pvis_vh2[ca][ivar][iiso][0][isf]->SetName(("h_low_mt_SS_pt_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            pvis_vh3[ca][ivar][iiso][0][isf]->SetName(("h_high_mt_OS_pt_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            pvis_vh4[ca][ivar][iiso][0][isf]->SetName(("h_high_mt_SS_pt_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            evis_vh1[ca][ivar][iiso][0][isf]->SetName(("h_low_mt_OS_eta_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            evis_vh2[ca][ivar][iiso][0][isf]->SetName(("h_low_mt_SS_eta_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            evis_vh3[ca][ivar][iiso][0][isf]->SetName(("h_high_mt_OS_eta_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            evis_vh4[ca][ivar][iiso][0][isf]->SetName(("h_high_mt_SS_eta_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            
                            mvis_vh1[ca][ivar][iiso][0][isf]->Write();
                            mvis_vh2[ca][ivar][iiso][0][isf]->Write(); 
                            mvis_vh3[ca][ivar][iiso][0][isf]->Write(); 
                            mvis_vh4[ca][ivar][iiso][0][isf]->Write(); 
                            pvis_vh1[ca][ivar][iiso][0][isf]->Write(); 
                            pvis_vh2[ca][ivar][iiso][0][isf]->Write(); 
                            pvis_vh3[ca][ivar][iiso][0][isf]->Write(); 
                            pvis_vh4[ca][ivar][iiso][0][isf]->Write(); 
                            evis_vh1[ca][ivar][iiso][0][isf]->Write(); 
                            evis_vh2[ca][ivar][iiso][0][isf]->Write(); 
                            evis_vh3[ca][ivar][iiso][0][isf]->Write(); 
                            evis_vh4[ca][ivar][iiso][0][isf]->Write();
                            
                        }//end isMT
                        if(isMM && !embe){
                            
                            mvis_vh1[ca][ivar][iiso][0][isf]->SetName(("h_OS_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            mvis_vh2[ca][ivar][iiso][0][isf]->SetName(("h_SS_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            pvis_vh1[ca][ivar][iiso][0][isf]->SetName(("h_OS_pt_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            pvis_vh2[ca][ivar][iiso][0][isf]->SetName(("h_SS_pt_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            evis_vh1[ca][ivar][iiso][0][isf]->SetName(("h_OS_eta_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());
                            evis_vh2[ca][ivar][iiso][0][isf]->SetName(("h_SS_eta_"+label_+varNames[ivar]+stringIsolation[iiso]+stringDataMG[isf]).c_str());

                            mvis_vh1[ca][ivar][iiso][0][isf]->Write();
                            mvis_vh2[ca][ivar][iiso][0][isf]->Write(); 
                            pvis_vh1[ca][ivar][iiso][0][isf]->Write(); 
                            pvis_vh2[ca][ivar][iiso][0][isf]->Write(); 
                            evis_vh1[ca][ivar][iiso][0][isf]->Write(); 
                            evis_vh2[ca][ivar][iiso][0][isf]->Write(); 
                           
                        }//end isMM
                    }//end loop over isf
                    if(isMM && embe){
                        for(size_t imce=0;imce<nmceToInclude;imce++){
                            
                            mvis_vh1[ca][ivar][iiso][imce][0]->SetName(("h_OS_"+label_+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]).c_str());
                            mvis_vh2[ca][ivar][iiso][imce][0]->SetName(("h_SS_"+label_+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]).c_str());
                            pvis_vh1[ca][ivar][iiso][imce][0]->SetName(("h_OS_pt_"+label_+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]).c_str());
                            pvis_vh2[ca][ivar][iiso][imce][0]->SetName(("h_SS_pt_"+label_+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]).c_str());
                            evis_vh1[ca][ivar][iiso][imce][0]->SetName(("h_OS_eta_"+label_+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]).c_str());
                            evis_vh2[ca][ivar][iiso][imce][0]->SetName(("h_SS_eta_"+label_+varNames[ivar]+stringIsolation[iiso]+stringEmbedding[imce]).c_str());

                            mvis_vh1[ca][ivar][iiso][imce][0]->Write();
                            mvis_vh2[ca][ivar][iiso][imce][0]->Write();
                            pvis_vh1[ca][ivar][iiso][imce][0]->Write();
                            pvis_vh2[ca][ivar][iiso][imce][0]->Write();
                            evis_vh1[ca][ivar][iiso][imce][0]->Write();
                            
                            hEmbWeight_SS_AntiIso->Write();
                            hEmbWeight_OS_AntiIso->Write();
                            hEmbWeight_SS_Iso->Write();
                            hEmbWeight_OS_Iso->Write();
                        }//end loop for embedding MM
                    }//end isMM
                }//end loop on isolation
            }//end loop on systematics
        }//end loop on categories
    }else{
        fout->cd();
        reweighting_3d_noMET->SetName(("rweight_3d_noMET_"+label_).c_str());
        reweighting_3d_EM_noMET->SetName(("rweight_3d_EM_noMET_"+label_).c_str());
        reweighting_3d_ET_noMET->SetName(("rweight_3d_ET_noMET_"+label_).c_str());
        reweighting_3d_MT_noMET->SetName(("rweight_3d_MT_noMET_"+label_).c_str());
        reweighting_3d_TT_noMET->SetName(("rweight_3d_TT_noMET_"+label_).c_str());
        reweighting_3d_noMET->Write();
        reweighting_3d_EM_noMET->Write();
        reweighting_3d_ET_noMET->Write();
        reweighting_3d_MT_noMET->Write();
        reweighting_3d_TT_noMET->Write();
    }

clock->Stop("<runZtautauAnalysis>");
clock->Show("<runZtautauAnalysis>");
delete clock;
fout->Close();

}

