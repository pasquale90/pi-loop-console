import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa

from paths import *
from data.note import Note
from data.midifile import Midifile



sample_midi_path=os.path.join(datapath,'drummer1/session1/106_funk_95_fill_4-4.mid')
sample_midi_path,sample_audio_path=sample_files()
mid = mido.MidiFile(sample_midi_path)#, clip=True

'''
"""# define classes"""
classes={"Bass":36,
        "Snare":38,
        "High_Tom":50,
        "Low_Mid_Tom":47,
        "High_Floor_Tom":43,
        "Open_Hi_Hat":46,
        "Closed_Hi_Hat":42,
        "Crash_Cymbal":49,
        "Ride_Cymbal":51}

def get_class_string(id):
  return [k for k,v in classes.items() if v == id][0]
'''

#test Midifile class
test_midi=Midifile(sample_midi_path)
#test_midi.parse_midi_messages()
print(test_midi.get_tempo())
print(test_midi.get_bpm())

#test Note class
num=test_midi.get_num_notes()
for i in range(num):
  n=test_midi.get_note(i)
  print(f'note {i+1}--> label:{n.get_label()},timestamp(ticks):{n.get_timestamp()}/{test_midi.get_total_ticks()}, \
  duration(ticks):{n.get_duration()}, \
  pitch:{n.get_pitch()}, \
  velocity:{n.get_velocity()} \
  timestamp(secs):{n.get_timestamp_sec()}/{test_midi.get_total_seconds()}, \
  duration(secs):{n.get_duration_sec()}')


"""# read midi files - csv"""

#read midi files using info.csv
csvpath=get_csv_path()#os.path.join(datapath,'info.csv')
csvfile=pd.read_csv(csvpath,header=None)
print(csvfile)
print(type(csvfile))

midipaths=[]
#read midi files by iteratin through the folders
for folder in os.listdir(datapath):
  print (f'folder {folder}')
  folderpath=os.path.join(datapath,folder)
  if os.path.isdir(folderpath):
    for session_folder in os.listdir(folderpath):
      print(f'\tsession_folder {session_folder}')
      session_folderpath=os.path.join(folderpath,session_folder)
      if os.path.isdir(session_folderpath):
        for midifile in os.listdir(session_folderpath):
          #print('\t\t\t',midifile)
          midipath=os.path.join(midifile,session_folder)
          if midifile[-4:]=='.mid':
            midipaths.append(midipath)
print(f'{len(midipaths)} midifiles are found')

"""# list all files"""
  
csv_path,data_folders=get_data_subpaths(datapath)
midipaths,audiopaths,bpms,durations,splits,beat_types,sessions=read_csv(csv_path)
#for subf in data_folders:
#  print(os.listdir(subf))

print(len(midipaths))
print(len(audiopaths))
print(len(list(set(midipaths))))
print(len(list(set(audiopaths))))
