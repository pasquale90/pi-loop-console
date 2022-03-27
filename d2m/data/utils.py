import os

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
