import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa
#from tqdm import tqdm
import math 

from paths import get_data_subpaths,get_csv,datapath
#from data import midifile,audio,utils,waveform,segment
import preprocess
import data #preprocess
  
if __name__=="__main__":
  
  #define paths - scan folders
  csv_path,folders=get_data_subpaths(datapath)
  csv=get_csv(csv_path)
  print(f'csv of shape \t {len(csv.index.values)} x {len(csv.columns.values)}')
  print(f'rows {csv.index.values}')
  print(f'columns {csv.columns.values}')
  
  
  # create segments - timestamp , note-label , duration , velocity 
  data, labels = preprocess.create_segments(csv,folders)
    
  # split data
  # ...
  train_data, train_labels = data,labels
  valid_data = [],[]
  
  #data.Data(train_data,labels)
  #valid_data = data.Data(csv,folders)
  
  
  