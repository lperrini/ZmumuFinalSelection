
#ifndef MainTreeHandler_H
#define MainTreeHandler_H

#include<iostream>
#include "TTree.h"

class MainTreeHandler
{
    public: 

        MainTreeHandler(TTree *tree, std::string channel, bool isMC);

        ~MainTreeHandler();
        int run, lumi, evt, NUP;
        bool secondMuon   = false;
        float lheHt; int lheNOutPartons;
        float npu, rho, npv=-1, puweight, weight;
        float trigweight_1, e_1, px_1, py_1, pz_1, pt_1, phi_1, eta_1, m_1, q_1, d0_1, dZ_1, mt_1, iso_1, l1_decayMode;
        bool id_m_highpt_1, id_m_loose_1, id_m_medium_1, id_m_tight_1, id_m_tightnovtx_1, id_m_hihghpt_1, id_e_mva_nt_loose_1, id_e_mva_nt_tight_1, id_e_cut_veto_1, id_e_cut_loose_1, id_e_cut_medium_1, id_e_cut_tight_1;
        float e_2, px_2, py_2, pz_2, pt_2, phi_2, eta_2, m_2, q_2, d0_2, dZ_2, mt_2, iso_2, l2_decayMode;
        bool id_m_highpt_2, id_m_loose_2, id_m_medium_2, id_m_tight_2, id_m_tightnovtx_2, id_m_hihghpt_2, id_e_mva_nt_loose_2, id_e_mva_nt_tight_2, id_e_cut_veto_2, id_e_cut_loose_2, id_e_cut_medium_2, id_e_cut_tight_2, againstElectronLooseMVA6_2, againstElectronMediumMVA6_2, againstElectronTightMVA6_2, againstElectronVLooseMVA6_2, againstElectronVTightMVA6_2, againstMuonLoose3_2, againstMuonTight3_2, byLooseCombinedIsolationDeltaBetaCorr3Hits_2, byMediumCombinedIsolationDeltaBetaCorr3Hits_2, byTightCombinedIsolationDeltaBetaCorr3Hits_2, byVLooseIsolationMVArun2v1DBoldDMwLT_2, byLooseIsolationMVArun2v1DBoldDMwLT_2, byMediumIsolationMVArun2v1DBoldDMwLT_2, byTightIsolationMVArun2v1DBoldDMwLT_2, byVTightIsolationMVArun2v1DBoldDMwLT_2;
        int decayModeFindingOldDMs_2, decayModeFindingNewDMs_2;
        float trigweight_2, byCombinedIsolationDeltaBetaCorrRaw3Hits_2, byIsolationMVA3oldDMwoLTraw_2, byIsolationMVA3oldDMwLTraw_2, byIsolationMVA3newDMwoLTraw_2, byIsolationMVA3newDMwLTraw_2, chargedIsoPtSum_2, neutralIsoPtSum_2, puCorrPtSum_2;

        float sigmaIetaIeta_1, deltaPhiSuperCluster_1, deltaEtaSuperCluster_1, depositR03tracker_1, depositR03ecal_1, depositR03hcal_1, trackIso_1, ecalIso_1, hcalIso_1;
        float sigmaIetaIeta_2, deltaPhiSuperCluster_2, deltaEtaSuperCluster_2, depositR03tracker_2, depositR03ecal_2, depositR03hcal_2, trackIso_2, ecalIso_2, hcalIso_2;
        float pt_tt, m_vis, ptvis;
        float m_sv_tr, m_sv_tr_tauUP,m_sv_tr_tauDOWN, m_sv, m_sv_tauUP,m_sv_tauDOWN,pt_sv,pt_sv_tauUP,pt_sv_tauDOWN,ptu_sv,ptu_sv_tauUP,ptu_sv_tauDOWN,eta_sv,eta_sv_tauUP,eta_sv_tauDOWN,etau_sv,etau_sv_tauUP,etau_sv_tauDOWN,phi_sv,phi_sv_tauUP, phi_sv_tauDOWN,phiu_sv,phiu_sv_tauUP,phiu_sv_tauDOWN,fitMETRho_sv,fitMETRho_sv_tauUP,fitMETRho_sv_tauDOWN,fitMETPhi_sv,fitMETPhi_sv_tauUP,fitMETPhi_sv_tauDOWN;
        float met_pt, met_phi, met_px, met_py, umet_px, umet_py;
        float mvamet, mvametphi, pzetavis, pzetamiss, mvacov00, mvacov01, mvacov11, mvacov10;
        int nb_extra_electrons, nb_extra_muons;
        float top_reweighting, gen_Z_pt, gen_Z_mass, gen_Z_eta;
        bool extraelec_veto, extramuon_veto, dilepton_veto;
        int ptype_1, ptype_2;
        int   gen_match_1, gen_match_2;
        float gen_match_px1, gen_match_py1, gen_match_pz1, gen_match_e1;
        float gen_match_px2, gen_match_py2, gen_match_pz2, gen_match_e2;
        float gen_match_tau_px1, gen_match_tau_py1, gen_match_tau_pz1, gen_match_tau_e1;
        float gen_match_tau_px2, gen_match_tau_py2, gen_match_tau_pz2, gen_match_tau_e2;

        std::vector<float>  *jets_px=0;                
        std::vector<float>  *jets_py=0;                
        std::vector<float>  *jets_pz=0;                
        std::vector<float>  *jets_e=0;                
        std::vector<float>  *jets_rawPt=0;
        std::vector<float>  *jets_area=0;
        std::vector<float>  *jets_mT=0;
        std::vector<int>    *jets_Flavour=0;
        std::vector<int>    *jets_HadronFlavour=0;
        std::vector<int>    *jets_genjetIndex=0;
        std::vector<float>  *jets_PUJetID=0;
        std::vector<float>  *jets_PUJetIDupdated=0;
        std::vector<float>  *jets_jecUnc=0;
        std::vector<float>  *bCSVscore=0;
        std::vector<int>    *PFjetID=0;
        std::vector<float>  *jetRawf=0;
        std::vector<float>  *genjet_px=0;
        std::vector<float>  *genjet_py=0;
        std::vector<float>  *genjet_pz=0;
        std::vector<float>  *genjet_e=0;
        std::vector<int>    *genjet_partonFlavour=0;
        std::vector<int>    *genjet_hadronFlavour=0;
    private:

};

#endif

