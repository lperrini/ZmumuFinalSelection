cmsrel CMSSW_7_6_3_patch2/src/ 
cd CMSSW_7_6_3_patch2/src 
cmsenv 
git clone https://github.com/lperrini/EmbeddingWeights.git 
mv EmbeddingWeights/* . 
rm -rf EmbeddingWeights

git clone https://github.com/CMS-HTT/LeptonEff-interface.git HTT-utilities 
cd HTT-utilities/LepEffInterface/ 
git clone https://github.com/CMS-HTT/LeptonEfficiencies.git data 
cd ${CMSSW_BASE}/src

git clone https://github.com/lperrini/ZmumuFinalSelection.git
mv ZmumuFinalSelection/* .
rm -rf ZmumuFinalSelection

scram b -j 9


###how to use code
###the main analysizer is UserCode/ztautau_fwk/bin/runZtautauAnalysis.cc
###to submit the code - for example - for mumu selection with the embedding
###weights for mutau

cd UserCode/ztautau_fwk/test
python submitJobs_ZtautauAnalysis_Embedding_MT.py

###output will be stored in UserCode/ztautau_fwk/data/ntuples/mt_embedding

cd UserCode/ztautau_fwk/data/ntuples/mt_embedding
hadd ZLL.root ZLL*
hadd ZTT.root ZTT*
hadd VV.root WW* WZ* ST* ZZ*
hadd EWKW.root EWKW*
hadd TT.root TT_*
hadd W.root W1Jets.root W2Jets.root W3Jets.root W4Jets.root WJets.root

###estimate background -

cd UserCode/ztautau_fwk/script

###edit EstimateBackground_Analysis_Embedding.py to check if it is poiting to
###the correct directory

python EstimateBackground_Analysis_Embedding.py

###edit Create_Datacards_Analysis_Embedding.py to check if it is poiting to
###the correct directory

python Create_Datacards_Analysis_Embedding.py

### the datacard obtained has to be copied in the /auxiliaries/shapes/TALLINN
### directory to start performing the combined fit!
