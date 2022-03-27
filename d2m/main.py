import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa
from tqdm import tqdm

from paths import *
from data.note import Note
from data.midifile import Midifile
from data.audio import Audio


#scan folders
csv_path,folders=get_data_subpaths(datapath)
csv=get_csv(csv_path)
#print(f'csv of shape \t {len(csv.index.values)} x {len(csv.columns.values)}')
#print(f'rows {csv.index.values}')
#print(f'columns {csv.columns.values}')

for i in tqdm(range (csv.shape[0])):
  midifile=csv['midi_filename'].loc[csv.index[i]]
  audiofile=csv['audio_filename'].loc[csv.index[i]]
  bpm=csv['bpm'].loc[i]

  midi=Midifile(get_abs_datapath(midifile))
  #audio=Audio(get_abs_datapath(audiofile),8000)
  
  correct,approx,errors=0,0,0
  bpm_diff=abs(midi.get_bpm()-bpm)
  if bpm_diff==1 or bpm_diff==2:
    print(f'APPROX midifile {midifile} --> bpm {midi.get_bpm()} != {bpm}')
    approx+=1
  elif (bpm_diff>2):
    print(f'ERROR midifile {midifile} --> bpm {midi.get_bpm()} != {bpm}')
    errors+=1
  else:
    correct+=1
  #break
print(f'from {csv.shape[0]} midifiles, there are {correct}({correct/csv.shape[0]*100}%) correct, {approx}({approx/csv.shape[0]*100}%) approx and {errors}({errors/csv.shape[0]*100}%) errors')


audiofiles, midifiles=pathscan(len(folders))
print(len(audiofiles), len(midifiles))