import os
import pandas as pd
import paths

num_folds=10
audiofiles,midifiles=[],[]

#def print_paths_demo():

def pathscan():
  for f in range(1,num_folds+1):
    if (f!=2):
      #print("\n\nFOLD /#"+str(f))
      #print(os.listdir(paths.get_fold_path(f)))
      foldpath=paths.get_fold_path(f)
      #print(f'foldpath {foldpath}')
      session_path=paths.get_session_path(foldpath)
      for sespath in session_path:
        curr_path=paths.set_session_path(foldpath,sespath)
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

def get_csv():
  csv=pd.read_csv(paths.csvpath)
  print(f'rows \t {csv.index.values}')
  print(f'columns \t {csv.columns.values}')
  
  return csv


def analyze_csv():
  #styles dist
  styles=csv['style'].values
  unq_styles=csv['style'].unique()
  #print(unq_styles)
  #print(type(styles))
  #print(len(styles))

  #beats per seconds dist
  bpms=csv['bpm'].values
  #print(type(bpms))
  #print(len(bpms))

  beatfill=csv['beat_type'].values
  #print(beatfill)
  unq_beatfill=csv['beat_type'].unique()
  #print(unq_beatfill)

  #Durations
  durations=csv['duration'].values
  unq_durations=csv['duration'].unique()
  print(len(durations))
  print(len(unq_durations))
  