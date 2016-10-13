###################################################################
#author L.Perrini
###################################################################

import os
import sys
import glob
import re

###################################################################
#### Parameters to be changed for each production

datasetsFile = "DATASETS.txt"

###################################################################
#### Automated script starting

# dataset block definition
#NumberFilesBegin = "==="
StorageDir = "/storage/data/cms/store/user/lperrini/eMiniAOD76X/"

# check if file with dataset exist
if not os.path.isfile(datasetsFile):
    print "File %s not found!!!" % datasetsFile
    sys.exit()

dtsetToCheck = []
numberFilesToCheck = []

print " =============================================== Starting checks =============================================== "

# READ INPUT FILE
with open(datasetsFile) as fIn:
    for line in fIn:
        line = line.strip() # remove newline at the end and leding/trailing whitespaces
 
        if not line: #skip empty lines
            continue

        #print line        
        words = line.split()
        dtsetToCheck.append(words[0])
        numberFilesToCheck.append(words[2])
        
ndim = len(dtsetToCheck) 
for i in range(0,ndim):
   dtsetName = dtsetToCheck[i]
   diff=0
   dtsetFilesInNtuples = int(len(glob.glob('ntuples_'+dtsetName+'/HTauTau_'+dtsetName+'*.root') ))
   if (dtsetFilesInNtuples<int(numberFilesToCheck[i])):
      diff = int(numberFilesToCheck[i])-dtsetFilesInNtuples
      print " %d still to run for  %s" % (diff,dtsetName)
   if (dtsetFilesInNtuples>int(numberFilesToCheck[i])):
      diff = dtsetFilesInNtuples-int(numberFilesToCheck[i])
      print " I would expect to fine %d more files in %s " % (diff,dtsetName)

 
