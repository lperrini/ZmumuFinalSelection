#include "UserCode/ztautau_fwk/interface/MainTreeHandler.h"

using namespace std;

MainTreeHandler::MainTreeHandler(TTree* tree, std::string channel, bool isMC)
{
    tree->SetBranchAddress("run", &run); 
    tree->SetBranchAddress("lumi", &lumi);
    tree->SetBranchAddress("evt", &evt);
    tree->SetBranchAddress("ptype_1", &ptype_1);
    tree->SetBranchAddress("ptype_2", &ptype_2);
    if(isMC){
        tree->SetBranchAddress("NUP", &NUP);
        tree->SetBranchAddress("weight", &weight); 
        tree->SetBranchAddress("lheHt", &lheHt); 
        tree->SetBranchAddress("lheNOutPartons", &lheNOutPartons); 
        tree->SetBranchAddress("gen_match_1", &gen_match_1); 
        tree->SetBranchAddress("gen_match_2", &gen_match_2); 
        tree->SetBranchAddress("gen_match_px1", &gen_match_px1); 
        tree->SetBranchAddress("gen_match_py1", &gen_match_py1); 
        tree->SetBranchAddress("gen_match_pz1", &gen_match_pz1); 
        tree->SetBranchAddress("gen_match_e1", &gen_match_e1); 
        tree->SetBranchAddress("gen_match_px2", &gen_match_px2); 
        tree->SetBranchAddress("gen_match_py2", &gen_match_py2); 
        tree->SetBranchAddress("gen_match_pz2", &gen_match_pz2); 
        tree->SetBranchAddress("gen_match_e2", &gen_match_e2); 
        tree->SetBranchAddress("gen_match_tau_px1", &gen_match_tau_px1); 
        tree->SetBranchAddress("gen_match_tau_py1", &gen_match_tau_py1); 
        tree->SetBranchAddress("gen_match_tau_pz1", &gen_match_tau_pz1); 
        tree->SetBranchAddress("gen_match_tau_e1", &gen_match_tau_e1); 
        tree->SetBranchAddress("gen_match_tau_px2", &gen_match_tau_px2); 
        tree->SetBranchAddress("gen_match_tau_py2", &gen_match_tau_py2); 
        tree->SetBranchAddress("gen_match_tau_pz2", &gen_match_tau_pz2); 
        tree->SetBranchAddress("gen_match_tau_e2", &gen_match_tau_e2); 
        tree->SetBranchAddress("gen_Z_pt", &gen_Z_pt); 
        tree->SetBranchAddress("gen_Z_mass", &gen_Z_mass);
        tree->SetBranchAddress("gen_Z_eta", &gen_Z_eta);
        tree->SetBranchAddress("genjet_px",&genjet_px);
        tree->SetBranchAddress("genjet_py",&genjet_py);
        tree->SetBranchAddress("genjet_pz",&genjet_pz);
        tree->SetBranchAddress("genjet_e",&genjet_e);
        tree->SetBranchAddress("genjet_partonFlavour",&genjet_partonFlavour);
        tree->SetBranchAddress("genjet_hadronFlavour",&genjet_hadronFlavour);
    } 
    tree->SetBranchAddress("secondMuon", &secondMuon); 
    tree->SetBranchAddress("dilepton_veto", &dilepton_veto); 
    tree->SetBranchAddress("extraelec_veto", &extraelec_veto); 
    tree->SetBranchAddress("extramuon_veto", &extramuon_veto); 

    tree->SetBranchAddress("puweight", &puweight); 

    tree->SetBranchAddress("npv", &npv); 
    tree->SetBranchAddress("npu", &npu); 
    tree->SetBranchAddress("rho", &rho); 

    tree->SetBranchAddress("pt_1", &pt_1); 
    tree->SetBranchAddress("px_1", &px_1); 
    tree->SetBranchAddress("py_1", &py_1); 
    tree->SetBranchAddress("pz_1", &pz_1); 
    tree->SetBranchAddress("e_1", &e_1); 
    tree->SetBranchAddress("phi_1", &phi_1);
    tree->SetBranchAddress("eta_1", &eta_1); 
    tree->SetBranchAddress("m_1", &m_1); 
    tree->SetBranchAddress("q_1", &q_1); 
    tree->SetBranchAddress("d0_1", &d0_1); 
    tree->SetBranchAddress("dZ_1", &dZ_1); 
    tree->SetBranchAddress("mt_1", &mt_1); 
    tree->SetBranchAddress("iso_1", &iso_1);
    tree->SetBranchAddress("l1_decayMode", &l1_decayMode); 
    tree->SetBranchAddress("id_m_loose_1", &id_m_loose_1); 
    tree->SetBranchAddress("id_m_medium_1", &id_m_medium_1); 
    tree->SetBranchAddress("id_m_tight_1", &id_m_tight_1); 
    tree->SetBranchAddress("id_m_tightnovtx_1", &id_m_tightnovtx_1); 
    tree->SetBranchAddress("id_m_highpt_1", &id_m_highpt_1); 
    tree->SetBranchAddress("id_e_mva_nt_loose_1", &id_e_mva_nt_loose_1); 
    tree->SetBranchAddress("id_e_mva_nt_tight_1", &id_e_mva_nt_tight_1); 
    tree->SetBranchAddress("id_e_cut_veto_1", &id_e_cut_veto_1); 
    tree->SetBranchAddress("id_e_cut_loose_1", &id_e_cut_loose_1); 
    tree->SetBranchAddress("id_e_cut_medium_1", &id_e_cut_medium_1); 
    tree->SetBranchAddress("id_e_cut_tight_1", &id_e_cut_tight_1);
    tree->SetBranchAddress("trigweight_1", &trigweight_1); 
    tree->SetBranchAddress("pt_2", &pt_2); 
    tree->SetBranchAddress("phi_2", &phi_2); 
    tree->SetBranchAddress("eta_2", &eta_2); 
    tree->SetBranchAddress("px_2", &px_2); 
    tree->SetBranchAddress("py_2", &py_2); 
    tree->SetBranchAddress("pz_2", &pz_2); 
    tree->SetBranchAddress("e_2", &e_2); 
    tree->SetBranchAddress("m_2", &m_2); 
    tree->SetBranchAddress("q_2", &q_2); 
    tree->SetBranchAddress("d0_2", &d0_2); 
    tree->SetBranchAddress("dZ_2", &dZ_2); 
    tree->SetBranchAddress("mt_2", &mt_2); 
    tree->SetBranchAddress("iso_2", &iso_2); 
    tree->SetBranchAddress("l2_decayMode", &l2_decayMode); 
    tree->SetBranchAddress("id_m_loose_2", &id_m_loose_2); 
    tree->SetBranchAddress("id_m_medium_2", &id_m_medium_2); 
    tree->SetBranchAddress("id_m_tight_2", &id_m_tight_2); 
    tree->SetBranchAddress("id_m_tightnovtx_2", &id_m_tightnovtx_2); 
    tree->SetBranchAddress("id_m_hihghpt_2", &id_m_highpt_2); 
    tree->SetBranchAddress("id_e_mva_nt_loose_2", &id_e_mva_nt_loose_2); 
    tree->SetBranchAddress("id_e_mva_nt_tight_2", &id_e_mva_nt_tight_2); 
    tree->SetBranchAddress("id_e_cut_veto_2", &id_e_cut_veto_2); 
    tree->SetBranchAddress("id_e_cut_loose_2", &id_e_cut_loose_2); 
    tree->SetBranchAddress("id_e_cut_medium_2", &id_e_cut_medium_2); 
    tree->SetBranchAddress("id_e_cut_tight_2", &id_e_cut_tight_2); 
    tree->SetBranchAddress("trigweight_2", &trigweight_2); 
    tree->SetBranchAddress("againstElectronLooseMVA6_2", &againstElectronLooseMVA6_2); 
    tree->SetBranchAddress("againstElectronMediumMVA6_2", &againstElectronMediumMVA6_2); 
    tree->SetBranchAddress("againstElectronTightMVA6_2", &againstElectronTightMVA6_2); 
    tree->SetBranchAddress("againstElectronVLooseMVA6_2", &againstElectronVLooseMVA6_2); 
    tree->SetBranchAddress("againstElectronVTightMVA6_2", &againstElectronVTightMVA6_2); 
    tree->SetBranchAddress("againstMuonLoose3_2", &againstMuonLoose3_2); 
    tree->SetBranchAddress("againstMuonTight3_2", &againstMuonTight3_2); 
    tree->SetBranchAddress("byLooseCombinedIsolationDeltaBetaCorr3Hits_2", &byLooseCombinedIsolationDeltaBetaCorr3Hits_2); 
    tree->SetBranchAddress("byMediumCombinedIsolationDeltaBetaCorr3Hits_2", &byMediumCombinedIsolationDeltaBetaCorr3Hits_2); 
    tree->SetBranchAddress("byTightCombinedIsolationDeltaBetaCorr3Hits_2", &byTightCombinedIsolationDeltaBetaCorr3Hits_2); 
    tree->SetBranchAddress("byCombinedIsolationDeltaBetaCorrRaw3Hits_2", &byCombinedIsolationDeltaBetaCorrRaw3Hits_2); 
    tree->SetBranchAddress("byVLooseIsolationMVArun2v1DBoldDMwLT_2", &byVLooseIsolationMVArun2v1DBoldDMwLT_2);
    tree->SetBranchAddress("byLooseIsolationMVArun2v1DBoldDMwLT_2", &byLooseIsolationMVArun2v1DBoldDMwLT_2);
    tree->SetBranchAddress("byMediumIsolationMVArun2v1DBoldDMwLT_2", &byMediumIsolationMVArun2v1DBoldDMwLT_2);
    tree->SetBranchAddress("byTightIsolationMVArun2v1DBoldDMwLT_2", &byTightIsolationMVArun2v1DBoldDMwLT_2);
    tree->SetBranchAddress("byVTightIsolationMVArun2v1DBoldDMwLT_2", &byVTightIsolationMVArun2v1DBoldDMwLT_2);
    tree->SetBranchAddress("byIsolationMVA3oldDMwoLTraw_2", &byIsolationMVA3oldDMwoLTraw_2); 
    tree->SetBranchAddress("byIsolationMVA3oldDMwLTraw_2", &byIsolationMVA3oldDMwLTraw_2); 
    tree->SetBranchAddress("byIsolationMVA3newDMwoLTraw_2", &byIsolationMVA3newDMwoLTraw_2); 
    tree->SetBranchAddress("byIsolationMVA3newDMwLTraw_2", &byIsolationMVA3newDMwLTraw_2); 
    tree->SetBranchAddress("chargedIsoPtSum_2", &chargedIsoPtSum_2); 
    tree->SetBranchAddress("decayModeFindingOldDMs_2", &decayModeFindingOldDMs_2); 
    tree->SetBranchAddress("decayModeFindingNewDMs_2", &decayModeFindingNewDMs_2); 
    tree->SetBranchAddress("neutralIsoPtSum_2", &neutralIsoPtSum_2);
    tree->SetBranchAddress("puCorrPtSum_2", &puCorrPtSum_2); 

    tree->SetBranchAddress("pt_tt", &pt_tt); 
    tree->SetBranchAddress("m_vis", &m_vis); 
    tree->SetBranchAddress("m_sv", &m_sv); 
    tree->SetBranchAddress("m_sv_tauUP", &m_sv_tauUP);
    tree->SetBranchAddress("m_sv_tauDOWN", &m_sv_tauDOWN); 
    tree->SetBranchAddress("pt_sv", &pt_sv); 
    tree->SetBranchAddress("ptu_sv", &ptu_sv); 
    tree->SetBranchAddress("pt_sv_tauUP", &pt_sv_tauUP); 
    tree->SetBranchAddress("pt_sv_tauDOWN", &pt_sv_tauDOWN);
    tree->SetBranchAddress("ptu_sv_tauUP", &ptu_sv_tauUP);
    tree->SetBranchAddress("ptu_sv_tauDOWN", &ptu_sv_tauDOWN);
    tree->SetBranchAddress("eta_sv", &eta_sv); 
    tree->SetBranchAddress("etau_sv", &etau_sv); 
    tree->SetBranchAddress("eta_sv_tauUP", &eta_sv_tauUP);
    tree->SetBranchAddress("eta_sv_tauDOWN", &eta_sv_tauDOWN); 
    tree->SetBranchAddress("etau_sv_tauUP", &etau_sv_tauUP); 
    tree->SetBranchAddress("etau_sv_tauDOWN", &etau_sv_tauDOWN); 
    tree->SetBranchAddress("phi_sv", &phi_sv);
    tree->SetBranchAddress("phi_sv_tauUP", &phi_sv_tauUP); 
    tree->SetBranchAddress("phi_sv_tauDOWN", &phi_sv_tauDOWN); 
    tree->SetBranchAddress("phiu_sv", &phiu_sv); 
    tree->SetBranchAddress("phiu_sv_tauUP", &phiu_sv_tauUP); 
    tree->SetBranchAddress("phiu_sv_tauDOWN", &phiu_sv_tauDOWN);
    tree->SetBranchAddress("fitMETRho_sv", &fitMETRho_sv); 
    tree->SetBranchAddress("fitMETRho_sv_tauUP", &fitMETRho_sv_tauUP); 
    tree->SetBranchAddress("fitMETRho_sv_tauDOWN", &fitMETRho_sv_tauDOWN); 
    tree->SetBranchAddress("fitMETPhi_sv", &fitMETPhi_sv); 
    tree->SetBranchAddress("fitMETPhi_sv_tauUP", &fitMETPhi_sv_tauUP); 
    tree->SetBranchAddress("fitMETPhi_sv_tauDOWN", &fitMETPhi_sv_tauDOWN); 
    tree->SetBranchAddress("m_sv_tr", &m_sv_tr); 
    tree->SetBranchAddress("m_sv_tr_tauUP", &m_sv_tr_tauUP); 
    tree->SetBranchAddress("m_sv_tr_tauDOWN", &m_sv_tr_tauDOWN);

    tree->SetBranchAddress("met_pt", &met_pt); 
    tree->SetBranchAddress("met_px", &met_px); 
    tree->SetBranchAddress("met_py", &met_py); 
    tree->SetBranchAddress("umet_px", &umet_px); 
    tree->SetBranchAddress("umet_py", &umet_py); 
    tree->SetBranchAddress("met_phi", &met_phi); 
    tree->SetBranchAddress("mvamet", &mvamet); 
    tree->SetBranchAddress("mvametphi", &mvametphi); 
    tree->SetBranchAddress("pzetavis", &pzetavis); 
    tree->SetBranchAddress("pzetamiss", &pzetamiss); 
    tree->SetBranchAddress("mvacov00", &mvacov00); 
    tree->SetBranchAddress("mvacov10", &mvacov10); 
    tree->SetBranchAddress("mvacov01", &mvacov01); 
    tree->SetBranchAddress("mvacov11", &mvacov11); 

    tree->SetBranchAddress("ptvis", &ptvis); 
    tree->SetBranchAddress("nb_extra_electrons", &nb_extra_electrons); 
    tree->SetBranchAddress("nb_extra_muons", &nb_extra_muons); 
    tree->SetBranchAddress("jets_px",&jets_px);
    tree->SetBranchAddress("jets_py",&jets_py);
    tree->SetBranchAddress("jets_pz",&jets_pz);
    tree->SetBranchAddress("jets_e",&jets_e);
    tree->SetBranchAddress("jets_rawPt",&jets_rawPt);
    tree->SetBranchAddress("jets_area",&jets_area);
    tree->SetBranchAddress("jets_mT",&jets_mT);
    tree->SetBranchAddress("jets_Flavour",&jets_Flavour);
    tree->SetBranchAddress("jets_HadronFlavour",&jets_HadronFlavour);
    tree->SetBranchAddress("jets_genjetIndex",&jets_genjetIndex);
    tree->SetBranchAddress("jets_PUJetID",&jets_PUJetID);
    tree->SetBranchAddress("jets_PUJetIDupdated",&jets_PUJetIDupdated);
    tree->SetBranchAddress("jets_jecUnc",&jets_jecUnc);
    tree->SetBranchAddress("bCSVscore",&bCSVscore);
    tree->SetBranchAddress("PFjetID",&PFjetID);
    tree->SetBranchAddress("jetRawf",&jetRawf);
    

}

MainTreeHandler::~MainTreeHandler()
{
}
