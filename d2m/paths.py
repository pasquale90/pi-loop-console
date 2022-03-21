import os

def sample_files():
  samplepath=PWD+"/samples/"
  sample="220_rock-halftime_140_fill_4-4_6"
  midisample=samplepath+sample+".midi"
  audiosample=samplepath+sample+".wav"
  return midisample,audiosample

def get_fold_path(fold):
  filepath=datapath+"drummer"+str(fold)+"/"
  return filepath

def set_session_path(foldpath,sessionpath):
  return foldpath+sessionpath

def get_session_path(foldpath):
  return os.listdir(foldpath)

PWD=os.getcwd()
datapath=PWD+"/e-gmd/"
csvpath=datapath+"e-gmd-v1.0.0.csv"

