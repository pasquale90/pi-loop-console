import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa
from tqdm import tqdm

from paths import *
from data import midifile,audio,utils,waveform

#scan folders
csv_path,folders=get_data_subpaths(datapath)
csv=get_csv(csv_path)
print(f'csv of shape \t {len(csv.index.values)} x {len(csv.columns.values)}')
print(f'rows {csv.index.values}')
print(f'columns {csv.columns.values}')

print_thress=100
correct,errors=0,0
for i in tqdm(range (csv.shape[0])):
  midi_relpath=csv['midi_filename'].loc[csv.index[i]]
  audio_relpath=csv['audio_filename'].loc[csv.index[i]]

  midi=midifile.Midifile(get_abs_datapath(midi_relpath))
  aud=audio.Audio(get_abs_datapath(audio_relpath),2000)
  raw=aud.get_raw()



  #"" temp
  savefpath=os.path.join('/data/pl/pi-loop-console/d2m/tempfiles_/audios/',str(i)+'.wav')
  print(savefpath)
  aud.temp_save(savefpath)
  
  #env=aud.get_envelop()
  wf = waveform.Waveform(get_abs_datapath(audio_relpath))
  #wf.save('/data/pl/pi-loop-console/d2m/tempfiles_/waveforms/',str(i)+'.png')
  #""


  if i>20:
    break

audiofiles, midifiles=utils.pathscan(len(folders))
print(len(audiofiles), len(midifiles))