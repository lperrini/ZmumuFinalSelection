cmsrel CMSSW_7_6_3_patch2/src/
cd CMSSW_7_6_3/src
cmsenv
git clone https://github.com/lperrini/EmbeddingWeights.git
mv EmbeddingWeights/* .
rm -rf EmbeddingWeights

git clone https://github.com/CMS-HTT/LeptonEff-interface.git HTT-utilities 
cd HTT-utilities/LepEffInterface/
git clone https://github.com/CMS-HTT/LeptonEfficiencies.git data 
cd ${CMSSW_BASE}/src

git clone https://github.com/lperrini/ZmumuFinalSelection.git

scram b -j 9





