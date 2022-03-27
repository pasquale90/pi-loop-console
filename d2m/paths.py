import os
import pandas as pd

PWD=os.getcwd()

datapath=os.path.join(PWD,"dataset/e-gmd")
csvpath=os.path.join(datapath,"e-gmd-v1.0.0.csv")

def sample_files():
  samplepath=os.path.join(PWD,"dataset/samples")
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

def get_csv(csvpath):
  return pd.read_csv(csvpath)

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

def read_csv(csv_path):
  midipaths,audiopaths,bpms,durations,splits,beat_types,sessions=[],[],[],[],[],[],[]
  csv=pd.read_csv(csv_path)  
  #print(csv.shape)
  idxs = csv.index.values.tolist()
  for i,idx in enumerate(idxs):
    #print(f'index {idx}, value {csv.loc[idx,:]}')
    midipaths.append(os.path.join(datapath,csv.loc[idx,"midi_filename"]))
    #audio_filename=csv.loc[idx,"audio_filename"])
    try:
      audiopaths.append(os.path.join(datapath,csv.loc[idx,"audio_filename"]))
    except TypeError:
      audiopaths.append("-")
    bpms.append(csv.loc[idx,"bpm"])
    durations.append(csv.loc[idx,"duration"])
    splits.append(csv.loc[idx,"split"])
    beat_types.append(csv.loc[idx,"beat_type"])
    sessions.append(csv.loc[idx,"session"])
  return midipaths,audiopaths,bpms,durations,splits,beat_types,sessions



#def print_paths_demo():
def get_fold_path(fold):
  filepath=os.path.join(datapath,"drummer"+str(fold))
  return filepath
def set_session_path(foldpath,sessionpath):
  return os.path.join(foldpath,sessionpath)
def get_session_path(foldpath):
  return os.listdir(foldpath)
def pathscan(num_folds=10):
  audiofiles,midifiles=[],[]
  for f in range(1,num_folds+1):
    if (f!=2):
      #print("\n\nFOLD /#"+str(f))
      #print(os.listdir(paths.get_fold_path(f)))
      foldpath=get_fold_path(f)
      #print(f'foldpath {foldpath}')
      session_path=get_session_path(foldpath)
      for sespath in session_path:
        curr_path=set_session_path(foldpath,sespath)
        #print(curr_audiopath)
        for file in os.listdir(curr_path):
          #print(f'audiofile:{file},\tcurr_audiopath:{curr_path},\tsespath:{sespath}')
          #print(audiofile)
          ext=file.split(".")[-1]
          if (ext=="wav"):
            audiofiles.append(curr_path+'/'+file)
          if (ext=="midi"):
            midifiles.append(curr_path+'/'+file)
  return audiofiles, midifiles

#method for returning abs path for each path readed from the csv
def get_abs_datapath(filepath):
  return os.path.join(datapath,filepath)