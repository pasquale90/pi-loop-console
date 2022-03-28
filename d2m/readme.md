# instructions for running
<br />
$ cd d2m <br />
$ mkdir dataset <br />
$ cd dataset <br />
$ wget https://storage.googleapis.com/magentadata/datasets/e-gmd/v1.0.0/e-gmd-v1.0.0.zip <br />
$ unzip e-gmd-v1.0.0.zip <br />
$ mv e-gmd-v1.0.0 e-gmd <br />
$ cd e-gmd <br />
$ wget https://storage.googleapis.com/magentadata/datasets/e-gmd/v1.0.0/e-gmd-v1.0.0.csv <br />
$ cd ../.. <br />
$ python main.py <br />

# next developing steps

## implemenent dataloader
- create segments (each file multiple individual segments)
- transform audio signal / peak detection
- form the relative_peaks features 
- define input(audio) and labels (midi)