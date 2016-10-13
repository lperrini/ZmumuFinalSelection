// class to derive event weights for b tag
// for more details check https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#b_tagging_efficiency_in_MC_sampl
#ifndef bTagSF_h
#define bTagSF_h

#include <string>
#include <vector>
#include <utility>
#include "BTagCalibrationStandalone.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"

class bTagSF
{
    public:
        TH2F *h_eff[3][3];
        TString WPs[3]={"L","M","T"};
        TString HFs[3]={"b","c","udsg"};
        enum WP {loose = 0, medium = 1, tight = 2};
        enum SFsyst {central = 0, up = 1, down = 2};

        bTagSF(std::string SFfilename);
        ~bTagSF();
        void init_efficiency(TString inputRootFile);
        float getEff (int jflav, WP wpt, float pt, float eta);
        float getSF (SFsyst syst, int jflav, WP wpt, float pt, float eta); // 0: central, 1: up, 2: down
        std::vector<float> getEvtWeight (std::vector<float> jets_csv, std::vector<TLorentzVector> jets, std::vector<int> jets_HF, int syst);

    private:
        BTagCalibration m_calib;

        // for b jets
        BTagCalibrationReader m_reader[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_up[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_do[3]; // 0: loose, 1: medium, 2: tight

        // for c jets
        BTagCalibrationReader m_reader_c[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_c_up[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_c_do[3]; // 0: loose, 1: medium, 2: tight

        // for udsg jets
        BTagCalibrationReader m_reader_udsg[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_udsg_up[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_udsg_do[3]; // 0: loose, 1: medium, 2: tight

        BTagCalibrationReader* m_readers [3][3][3]; // [b, c, udsg] [central, up, down] [loose, medium, tight] 

};

#endif
