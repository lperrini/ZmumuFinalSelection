#include "UserCode/ztautau_fwk/interface/bTagSF.h"
#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include "TString.h"
#include "TMath.h"

using namespace std;

bTagSF::bTagSF(std::string SFfilename) :
    
    m_calib("CSVv2", SFfilename.c_str()) ,
    m_reader {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "mujets", "central"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "mujets", "central"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "mujets", "central")} ,
    m_reader_up {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "mujets", "up"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "mujets", "up"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "mujets", "up")} ,
    m_reader_do {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "mujets", "down"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "mujets", "down"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "mujets", "down")} ,
    
    m_reader_c {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "mujets", "central"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "mujets", "central"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "mujets", "central")} ,
    m_reader_c_up {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "mujets", "up"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "mujets", "up"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "mujets", "up")} ,
    m_reader_c_do {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "mujets", "down"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "mujets", "down"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "mujets", "down")} ,

    m_reader_udsg {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "incl", "central"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "incl", "central"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "incl", "central")} ,
    m_reader_udsg_up {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "incl", "up"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "incl", "up"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "incl", "up")} ,
    m_reader_udsg_do {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "incl", "down"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "incl", "down"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "incl", "down")} 
{
    // // [b, c, udsg] [central, up, down] [loose, medium, tight]        = & _up//down [loose, medium, tight]

    m_readers[0][0][0] = & (m_reader[0]) ;
    m_readers[0][0][1] = & (m_reader[1]) ;
    m_readers[0][0][2] = & (m_reader[2]) ;

    m_readers[0][1][0] = & (m_reader_up[0]) ;
    m_readers[0][1][1] = & (m_reader_up[1]) ;
    m_readers[0][1][2] = & (m_reader_up[2]) ;

    m_readers[0][2][0] = & (m_reader_do[0]) ;
    m_readers[0][2][1] = & (m_reader_do[1]) ;
    m_readers[0][2][2] = & (m_reader_do[2]) ;


    m_readers[1][0][0] = & (m_reader_c[0]) ;
    m_readers[1][0][1] = & (m_reader_c[1]) ;
    m_readers[1][0][2] = & (m_reader_c[2]) ;

    m_readers[1][1][0] = & (m_reader_c_up[0]) ;
    m_readers[1][1][1] = & (m_reader_c_up[1]) ;
    m_readers[1][1][2] = & (m_reader_c_up[2]) ;

    m_readers[1][2][0] = & (m_reader_c_do[0]) ;
    m_readers[1][2][1] = & (m_reader_c_do[1]) ;
    m_readers[1][2][2] = & (m_reader_c_do[2]) ;


    m_readers[2][0][0] = & (m_reader_udsg[0]) ;
    m_readers[2][0][1] = & (m_reader_udsg[1]) ;
    m_readers[2][0][2] = & (m_reader_udsg[2]) ;

    m_readers[2][1][0] = & (m_reader_udsg_up[0]) ;
    m_readers[2][1][1] = & (m_reader_udsg_up[1]) ;
    m_readers[2][1][2] = & (m_reader_udsg_up[2]) ;
        
    m_readers[2][2][0] = & (m_reader_udsg_do[0]) ;
    m_readers[2][2][1] = & (m_reader_udsg_do[1]) ;
    m_readers[2][2][2] = & (m_reader_udsg_do[2]) ;

}

float bTagSF::getSF (SFsyst syst, int jflav, WP wpt, float pt, float eta)
{
    float SF = 1.0;
    BTagEntry::JetFlavor flav=BTagEntry::FLAV_B;
    int myWPIndex = (int) wpt;
    int myFlavIndex = -1; // indexes in the m_readers array
    int mySystIndex = (int) syst;

    if (abs(jflav) == 5)
    {
        flav = BTagEntry::FLAV_B;
        myFlavIndex = 0;
    }
    else if (abs(jflav) == 4)
    {
        flav = BTagEntry::FLAV_C;
        myFlavIndex = 1;
    }
    else if (abs(jflav) == 0)
    {
        flav = BTagEntry::FLAV_UDSG;
        myFlavIndex = 2;
    }

    SF = m_readers[myFlavIndex][mySystIndex][myWPIndex]->eval(flav, eta, pt);

    return SF;
}

void bTagSF::init_efficiency(TString inputRootFile){
    TFile * fileIn = new TFile(inputRootFile, "read");
    if (fileIn->IsZombie() ) { 
	    exit(1); 
    };

    for (int iWP = 0; iWP < 3; iWP++){
	    for (int iHF = 0; iHF < 3; iHF++){
                    TString name = Form("btagEff_%s_%s", HFs[iHF].Data(), WPs[iWP].Data());
		    h_eff[iWP][iHF] = (TH2F*)fileIn->Get(name.Data());
	    }
    }

    return;
}

float bTagSF::getEff (int jflav, WP wpt, float pt, float eta)
{
	int iHF=-999;
        float eff = 1.0;

	if (abs(jflav) == 5) iHF = 0;
	else if (abs(jflav) == 4) iHF = 1;
	else if (abs(jflav) == 0) iHF = 2;

	float aEta = TMath::Abs(eta);
	TH2F* h = h_eff[(int)wpt][iHF];
	int binglobal = h->FindBin (pt, aEta);
	int binx, biny, binz;
	h->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
	int nx = h->GetNbinsX();
	int ny = h->GetNbinsY();

	// under-overflows
	if (binx < 1) binx = 1;
	if (biny < 1) biny = 1;
	if (binx > nx) binx = nx;
	if (biny > ny) biny = ny;

	eff = h->GetBinContent (binx, biny);

	// protection againts wrongly measured efficiencies (low stat) --> reduce pT bin
	while (eff < 0.00000000001 && binx > 0)
	{
		binx-- ;
		eff = h->GetBinContent (binx, biny);
	}

	return eff;
}

// the collection jets_and_btag in input contain all the final list of jets, already cleaned from PU and leptons
// returns a collection of weights according to the tested WP
vector<float> bTagSF::getEvtWeight (std::vector<float> jets_csv, std::vector<TLorentzVector> jets, std::vector<int> jets_HF, int syst)
  {

  std::vector<float> P_MC   (3, 1.0); // 0 = L, 1 = M, 2 = T
  std::vector<float> P_Data (3, 1.0); // 0 = L, 1 = M, 2 = T

  TLorentzVector vJet (0,0,0,0);
  float WPtag[3] = {0.460, 0.800, 0.935}; // L, M, T
    
    for (unsigned int ijet = 0; ijet < jets_csv.size(); ijet++)
    {
        
        vJet = jets.at(ijet);
        //cout << " size " << jets_csv.size() << endl;

        int flav = jets_HF.at(ijet);
        if(flav<0) continue;

        float SF[3];
     
        if(syst==0)  {SF[0] = getSF (central, flav, loose, vJet.Pt(), vJet.Eta()); 
                      SF[1] = getSF (central, flav, medium, vJet.Pt(), vJet.Eta()); 
                      SF[2] = getSF (central, flav, tight, vJet.Pt(), vJet.Eta());}
        if(syst==1)  {SF[0] = getSF (up, flav, loose, vJet.Pt(), vJet.Eta()); 
                      SF[1] = getSF (up, flav, medium, vJet.Pt(), vJet.Eta()); 
                      SF[2] = getSF (up, flav, tight, vJet.Pt(), vJet.Eta());}
        if(syst==2)  {SF[0] = getSF (down, flav, loose, vJet.Pt(), vJet.Eta()); 
                      SF[1] = getSF (down, flav, medium, vJet.Pt(), vJet.Eta()); 
                      SF[2] = getSF (down, flav, tight, vJet.Pt(), vJet.Eta());}

        //cout << " ONLY JETS OF INDEX " << ijet << " HAVE PASSED " << endl;
        //cout << "idx jet " << ijet << " pt " << vJet.Pt() << " eta " << vJet.Eta() << " flav " << jets_HF.at(ijet) << endl;
        //cout << "scale factor L " << SF[0] << " M " << SF[1] << " T " << SF[2] << endl;

        float effBTag[3];
        effBTag[0] = getEff (flav, static_cast<WP> (0), vJet.Pt(), vJet.Eta()) ;
        effBTag[1] = getEff (flav, static_cast<WP> (1), vJet.Pt(), vJet.Eta()) ;
        effBTag[2] = getEff (flav, static_cast<WP> (2), vJet.Pt(), vJet.Eta()) ;

        //cout << "efficiency L " << effBTag[0] << " M " << effBTag[1] << " T " << effBTag[2] << endl;

        float CSV = jets_csv.at(ijet);
        bool tagged[3];
        tagged[0] = (CSV > WPtag[0]);
        tagged[1] = (CSV > WPtag[1]);
        tagged[2] = (CSV > WPtag[2]);
	
        //cout << "tagged[0] "  << tagged[0] << " tagged[1] " << tagged[1] << " tagged[2] "<<  tagged[2] << endl;

        for (int iWP = 0; iWP < 3; iWP++)
        {
            float tmpMC       = P_MC.at(iWP);
            float tmpData     = P_Data.at(iWP);

            if (tagged[iWP])
            {

                tmpMC       *= effBTag[iWP];  
                tmpData     *= effBTag[iWP]*SF[iWP];  
                //cout << " in tagged --> WP " << iWP << " tmpMC " << tmpMC << " tmpData " << tmpData << endl;
            } 
            else
            {
                tmpMC       *= (1. - effBTag[iWP]);  
                tmpData     *= (1. - (effBTag[iWP]*SF[iWP]) );  

                //cout << " in NOT tagged --> WP " << iWP << " tmpMC " << tmpMC << " tmpData " << tmpData << endl;
            }
        
            P_MC.at(iWP)       = tmpMC;
            P_Data.at(iWP)     = tmpData;

        }
    }
    // return ratio
    std::vector<float> weight(3);
    weight.at(0) = P_Data.at(0) / P_MC.at(0);
    weight.at(1) = P_Data.at(1) / P_MC.at(1);
    weight.at(2) = P_Data.at(2) / P_MC.at(2);
    //cout << "P_Data.at(0) " << P_Data.at(0) << " P_MC.at(0) " << P_MC.at(0) << " weight " << weight.at(0) << endl;
    //cout << "P_Data.at(1) " << P_Data.at(1) << " P_MC.at(1) " << P_MC.at(1) << " weight " << weight.at(1) << endl;
    //cout << "P_Data.at(2) " << P_Data.at(2) << " P_MC.at(2) " << P_MC.at(2) << " weight " << weight.at(2) << endl;
    
    //if (weight.at(0) < 0.05) 
    //{
    //    cout << "------ NONONO Null weight!!" << endl;
    //}
    return weight;
}



