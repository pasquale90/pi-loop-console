import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa
#from tqdm import tqdm
import math 

import paths
#from data import midifile,audio,utils,waveform,segment
import preprocess
import data #preprocess
  
if __name__=="__main__":
  
  #define paths - scan folders
  csv_path,folders=paths.get_data_subpaths(paths.datapath)
  csv=paths.get_csv(csv_path)
  print(f'csv of shape \t {len(csv.index.values)} x {len(csv.columns.values)}')
  print(f'rows {csv.index.values}')
  print(f'columns {csv.columns.values}')
  
  
  # create segments - timestamp , note-label , duration , velocity 
  audiolist, labels, velocities = preprocess.create_segments(csv,temp_print=True)
  
  
  print(len(audiolist), len(labels), len(velocities))
  print(audiolist[0], labels[0], velocities[0])
  
  """ 
  # split data
  # ...
  train_data, train_labels = audiolist,labels
  valid_data = [],[]
  
  data.Data(train_data,labels)
  #valid_data = data.Data(csv,folders)
  """
  
  