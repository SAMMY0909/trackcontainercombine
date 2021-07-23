```
setupATLAS --quiet
lsetup git
mkdir cont
cd cont
git atlas init-workdir https://:@gitlab.cern.ch:8443/atlas/athena.git
cd athena
git checkout release/22.6.3
git atlas addpkg DerivationFrameworkExamples
\cp -r ../patch/PhysicsAnalysis .
cd ../
mkdir build
cd build/
asetup Athena,master,latest
cmake ../athena/Projects/WorkDir/
make
source x86_64-centos7-gcc8-opt/setup.sh 
cd ../
mkdir run
cd run
Reco_tf.py --maxEvents 10 --inputAODFile /afs/cern.ch//work/k/khanov/public/valid1.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.recon.AOD.e4993_s3227_r12581/AOD.25384333._000688.pool.root.1 --outputDAODFile test.pool.root --reductionConf TEST1

```
