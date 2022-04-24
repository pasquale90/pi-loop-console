import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa
#from tqdm import tqdm
import math 

from paths import get_data_subpaths,get_csv,datapath
#from data import midifile,audio,utils,waveform,segment
import preprocess
  
if __name__=="__main__":

  #scan folders
  csv_path,folders=get_data_subpaths(datapath)
  csv=get_csv(csv_path)
  print(f'csv of shape \t {len(csv.index.values)} x {len(csv.columns.values)}')
  print(f'rows {csv.index.values}')
  print(f'columns {csv.columns.values}')
  
  preprocess.create_segments(csv,folders)