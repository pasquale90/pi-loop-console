# instructions for running
<br />
$ cd d2m <br />
$ mkdir dataset
$ cd dataset
$ wget https://storage.googleapis.com/magentadata/datasets/e-gmd/v1.0.0/e-gmd-v1.0.0.zip
$ unzip e-gmd-v1.0.0.zip
$ mv e-gmd-v1.0.0 e-gmd
$ cd e-gmd
$ wget https://storage.googleapis.com/magentadata/datasets/e-gmd/v1.0.0/e-gmd-v1.0.0.csv
$ cd ../..
$ python main.py <br />

# next developing steps

## implemenent dataloader
- create segments (each file multiple individual segments)
- transform audio signal / peak detection
- form the relative_peaks features 
- define input(audio) and labels (midi)