import os

PWD=os.getcwd()
datapath=os.path.join(PWD,"dataset/e-gmd")
csvpath=os.path.join(datapath,"e-gmd-v1.0.0.csv")

def sample_files():
  samplepath=os.path.join(PWD,"dataset/samples")
  sample="220_rock-halftime_140_fill_4-4_6"
  midisample=os.path.join(samplepath,sample+".midi")
  audiosample=os.path.join(samplepath,sample+".wav")
  return midisample,audiosample

def get_fold_path(fold):
  filepath=os.path.join(datapath,"drummer"+str(fold))
  return filepath

def set_session_path(foldpath,sessionpath):
  return os.path.join(foldpath,sessionpath)

def get_session_path(foldpath):
  return os.listdir(foldpath)

