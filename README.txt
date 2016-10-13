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


###HOW TO RUN THE ANALYSIS
###the main analysizer is UserCode/ztautau_fwk/bin/runZtautauAnalysis.cc
###to submit the code - for example - for mumu selection with the embedding
###weights for mutau

cd UserCode/ztautau_fwk/test
python submitJobs_ZtautauAnalysis_Embedding_MT.py

###the embedding jobs are splitted by number of events given that the jobs are
###are quite heavy
###to check that all the jobs submitted have finished do the following:
cd UserCode/ztautau_fwk/test
python checkRunningEmbedding_II.py
###you will see printouts on the screen that will inform you about the status
###of the jobs. Always open checkRunningEmbedding_II.py before running it in
###order to be sure it is poiting to the correct directory. Once all the jobs are
###done, you go to the directory where the output root files are stored:

cd UserCode/ztautau_fwk/data/ntuples/mt_embedding

###you need to hadd the files in order to respect the structure of the
###processes used to build the final datacards

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

###  NB: #########################################################
###in case you have to run jobs without embedding, simply use

cd UserCode/ztautau_fwk/test
submitJobs_ZtautauAnalysis_NoEmbedding.py

###edit always the file before running to check if you are running the mutau
###selection or the mumu selection. Additional instructions are included in
###the file itself.

###  NB: #########################################################
###in case you have to run jobs to estimate the data/MC corrections, simply use
cd UserCode/ztautau_fwk/test
python submitJobs_ZtautauAnalysis_ComputeWeights.py
cd UserCode/ztautau_fwk/data/ntuples/mm_weights

hadd ZLL.root ZLL*
hadd ZTT.root ZTT*
hadd VV.root WW* WZ* ST* ZZ*
hadd EWKW.root EWKW*
hadd TT.root TT_*
hadd W.root W1Jets.root W2Jets.root W3Jets.root W4Jets.root WJets.root

###estimate data/MC correction file

cd UserCode/ztautau_fwk/data/amcatnlo/
python Determine_DataMadgraphSF.py

### the root file obtained 'reweighting.root' will be used as input in the
###main analysis code
