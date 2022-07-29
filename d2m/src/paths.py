import os

#Set the value of the CWD variable accordingly
CWD=os.getcwd()
adaptive_CWD=CWD.split('pi-loop-console/d2m')[0]

ROOT=os.path.join(adaptive_CWD,"pi-loop-console/d2m")
datapath=os.path.join(ROOT,"datasets/e-gmd")
csvpath=os.path.join(datapath,"e-gmd-v1.0.0.csv")

def sample_files():
  samplepath=os.path.join(ROOT,"datasets/samples")
  sample="220_rock-halftime_140_fill_4-4_6"
  #sample='1_funk_80_beat_4-4_8'
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

def get_csv_path():
  return os.path.join(datapath,'e-gmd-v1.0.0.csv')

def get_data_subpaths(datapath):
  folders=[]
  for c in os.listdir(datapath):
    subpath=os.path.join(datapath,c)
    if (os.path.isdir(subpath)):
      #print(f'{subpath} is a folder')
      folders.append(subpath)
    elif (c=="e-gmd-v1.0.0.csv"):
      #print(f"{subpath} is the annotation file")
      csv_path=subpath
    else:
      #print(f'{c} is an irrelevant file - not of my concern!')
      pass
  return csv_path,folders

#def print_paths_demo():
def get_fold_path(fold):
  filepath=os.path.join(datapath,"drummer"+str(fold))
  return filepath
def set_session_path(foldpath,sessionpath):
  return os.path.join(foldpath,sessionpath)
def get_session_path(foldpath):
  return os.listdir(foldpath)

#method for returning abs path for each path readed from the csv
def get_abs_datapath(filepath):
  return os.path.join(datapath,filepath)