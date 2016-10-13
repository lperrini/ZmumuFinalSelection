cd tmp/WZ2L2Q
bsub -q 1nd -o logs job1396.sh
bsub -q 1nd -o logs job27.sh
bsub -q 1nd -o logs job542.sh
bsub -q 1nd -o logs job675.sh

cd ../ZLL1
bsub -q 1nd -o logs job105.sh

