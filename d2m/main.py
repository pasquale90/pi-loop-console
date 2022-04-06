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

  midi_tempo=midi.get_tempo()
  midi_bpm=midi.get_bpm()
  midi_rythm=midi.get_rythm_numerator()
  midi_denom=midi.get_rythm_denominator()
  midi_note0=midi.get_note(0)
  midi_numnotes=midi.get_num_notes()
  midi_totalticks=midi.get_total_ticks()
  midi_totalseconds=midi.get_total_seconds()
  midi_mspertick=midi.get_ms_per_tick()
  midi_convert_tick_to_second=midi.convert_tic2sec(512)
  midi_ppq=midi.get_ppq()
  
  print(f"midi_tempo {midi_tempo}")
  print(f"midi_bpm {midi_bpm}")
  print(f"midi_rythm {midi_rythm}")
  print(f"midi_denom {midi_denom}")
  print(f"midi_note0 {midi_note0}")
  print(f"midi_numnotes {midi_numnotes}")
  print(f"midi_totalticks {midi_totalticks}")
  print(f"midi_totalseconds {midi_totalseconds}")
  print(f"midi_mspertick {midi_mspertick}")
  print(f"midi_convert_tick_to_second {midi_convert_tick_to_second}")
  print(f"midi_ppq {midi_ppq}")
  
  audio_sr=aud.get_sampling_rate()
  audio_ismono=aud.is_mono()
  audio_duration=aud.get_duration_sec()
  print(f"audio_sr {audio_sr}")
  print(f"audio_ismono {audio_ismono}")
  print(f"audio_duration {audio_duration}")


  """ temp
  savefpath=os.path.join('/data/pl/pi-loop-console/d2m/tempfiles_/audios/',str(i)+'.wav')
  print(savefpath)
  aud.temp_save(savefpath)
  
  #env=aud.get_envelop()
  wf = waveform.Waveform(get_abs_datapath(audio_relpath))
  #wf.save('/data/pl/pi-loop-console/d2m/tempfiles_/waveforms/',str(i)+'.png')
  """


  if i>2:
    break

audiofiles, midifiles=utils.pathscan(len(folders))
print(len(audiofiles), len(midifiles))