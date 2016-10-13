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

void getYields(){
    string files[] = {"DYJetsLowMass.root",
        "DY1Jets.root",
        "DY2Jets.root",
        "DY3Jets.root",
        "DY4Jets.root",
        "EWKWMinus.root",
        "EWKWPlus.root",
        "EWKZToLL.root",
        "GluGluHToTauTau_M125.root",
        "ST_tW_antitop.root",
        "ST_tW_top.root",
        "ST_t_antitop.root",
        "ST_t_top.root",
        "SingleMuon.root",
        "VBFHToTauTau_M125.root",
        "W1Jets.root",
        "W2Jets.root",
        "W3Jets.root",
        "W4Jets.root",
        "WJets.root",
        "WW.root",
        "WZJets.root",
        "WZTo1L1Nu2Q.root",
        "WZTo1L3Nu.root",
        "WZTo2L2Q.root",
        "ZZTo2L2Q.root",
        "ZZTo4L.root",
        "DYJets_1.root",
        "DYJets_2.root",
        "DYJets_3.root",
        "DYJets_4.root",
        "DYJets_5.root",
        "DYJets_6.root",
        "DYJets_7.root",
        "DYJets_8.root",
        "DYJets_9.root",
        "DYJets_10.root",
        "DYJets_11.root",
        "DYJets_12.root",
        "DYJets_13.root",
        "DYJets_14.root",
        "DYJets_15.root",
        "DYJets_16.root",
        "DYJets_17.root",
        "DYJets_18.root",
        "DYJets_19.root",
        "DYJets_20.root",
        "DYJets_21.root",
        "DYJets_22.root",
        "DYJets_23.root",
        "DYJets_24.root",
        "DYJets_25.root",
        "TT_1.root",
        "TT_2.root",
        "TT_3.root",
        "TT_4.root",
        "TT_5.root",
        "TT_6.root",
        "TT_7.root",
        "TT_8.root",
        "TT_9.root",
        "TT_10.root",
        "TT_11.root",
        "TT_12.root",
        "TT_13.root",
        "TT_14.root",
        "TT_15.root"
         };

    int nfiles=67;//8;
    int countTT2=0; int countTT3=0;
    int countDY2=0; int countDY3=0;
    int countPreselected2=0; int countPreselected3=0;
    int countPreselectedDY2=0; int countPreselectedDY3=0;
    string sample;

    for(int i=0;i<nfiles;i++){
        TFile *f2_ = TFile::Open(("root://eoscms.cern.ch//eos/cms//store/user/veelken/Lucia/preselected_V2/"+files[i]).c_str());
        TFile *f3_ = TFile::Open(("root://eoscms.cern.ch//eos/cms//store/user/veelken/Lucia/preselected_V3/"+files[i]).c_str());
        TTree* t2  = (TTree*)  f2_->Get("RLE_tree");
        TTree* t3  = (TTree*)  f3_->Get("RLE_tree");
        TH1F* nbevt2 = (TH1F*)  f2_->Get("nbevt");
        TH1F* nbevt3 = (TH1F*)  f3_->Get("nbevt");

        sample = f3_->GetName();

        if (sample.find("DYJets_")!=std::string::npos){
            countDY2 = countDY2+nbevt2->GetBinContent(1);
            countDY3 = countDY3+nbevt3->GetBinContent(1);
        }
        if (sample.find("TT_")!=std::string::npos){
            countTT2 = countTT2+nbevt2->GetBinContent(1);
            countTT3 = countTT3+nbevt3->GetBinContent(1);
        }
     
        //if (sample.find("DYJets")!=std::string::npos || sample.find("DY1Jets")!=std::string::npos ||
        //        sample.find("DY2Jets")!=std::string::npos || sample.find("DY3Jets")!=std::string::npos ||
        //        sample.find("DY4Jets")!=std::string::npos) countPreselectedDY = countPreselectedDY + t->GetEntries()*2;
        //else
        //    countPreselected=countPreselected+t->GetEntries();
        cout << "sample "<< f2_->GetName() << " entries original V2 " << setprecision(10) << nbevt2->GetBinContent(1) << //" entries preselected " << t->GetEntries() << endl;
        " entries original V3 " << setprecision(10) << nbevt3->GetBinContent(1) << endl;//" entries preselected " << t->GetEntries() << endl;
    }

    //cout << "preselected "<< double((countPreselected+countPreselectedDY)/5000) << endl;
    cout << " total DYJets_ V2 " << countDY2 << " total DYJets_ V3 " << countDY3 <<  endl; 
    cout << " total TTJets_ V2 " << countTT2 << " total TTJets_ V3 " << countTT3 <<  endl; 
}
