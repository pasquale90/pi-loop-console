import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa

from paths import *
from data.note import Note
from data.midifile import Midifile
from data.audio import Audio



#GET SAMPLE FILES - AUDIO AND MIDI
sample_midi_path,sample_audio_path=sample_files()

#test Midifile class
print('TESTING MIDIFILE CLASS')
test_midi=Midifile(sample_midi_path)
#test_midi.parse_midi_messages()
print(f'tempo {test_midi.get_tempo()}')
print(f'bpm {test_midi.get_bpm()}')
print(f'Total seconds:{test_midi.get_total_seconds()}')

#test Note class
print('TESTING NOTE CLASS')
num=test_midi.get_num_notes()
print(f'num {num}')
for i in range(num):
  n=test_midi.get_note(i)
  print(f'note {i+1}--> label:{n.get_label()},timestamp(ticks):{n.get_timestamp()}/{test_midi.get_total_ticks()}, \
  duration(ticks):{n.get_duration()}, \
  pitch:{n.get_pitch()}, \
  velocity:{n.get_velocity()} \
  timestamp(secs):{n.get_timestamp_sec()}/{test_midi.get_total_seconds()}, \
  duration(secs):{n.get_duration_sec()}')


#test audio class
print('TESTING AUDIO CLASS')
aud = Audio(sample_audio_path,8000)
print(f'sr : {aud.get_sampling_rate()}')
print(f'mono: {aud.is_mono()}')
aud_sample=aud.get_sample()
print(f'sample shape {aud_sample.shape}')
print(f'duration seconds {aud.get_duration_sec()}')
