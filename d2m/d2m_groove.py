import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa

from paths import *

#sample_midi_path=os.path.join(datapath,'drummer1/session1/106_funk_95_fill_4-4.mid')
sample_midi_path,sample_audio_path=sample_files()
mid = mido.MidiFile(sample_midi_path)#, clip=True
'''
print(mid,"\n")
for i,track in enumerate(mid.tracks):
    print(f'track_{i}:\n {track}')
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

print(get_class_string(36))


#make a Note class
class Note:
  def __init__(self,channel,pitch,velocity,timestamp,timestamp_sec,duration,duration_sec):
    self.channel=channel
    self.pitch=self.remap_pitch(pitch)
    self.label=self.set_class(self.pitch)
    self.velocity=velocity
    self.timestamp=timestamp
    self.timestamp_sec=timestamp_sec
    self.duration=duration
    self.duration_sec=duration_sec
    
    self.relative_time=0 #in which part of the quarted is the note located

  def get_label(self):
    return self.label
  def get_pitch(self):
    return self.pitch
  def get_duration(self):
    return self.duration
  def get_velocity(self):
    return self.velocity
  def get_timestamp(self):
    return self.timestamp
  def get_timestamp_sec(self):
    return self.timestamp_sec
  def get_duration_sec(self):
    return self.duration_sec

  def remap_pitch(self,pitch):
    if pitch==36:
      return 36#"Bass"
    elif pitch in (37,38,40):
      return 38#"Snare"
    elif pitch in (48,50):
      return 50#"High_Tom"
    elif pitch in (45,47):
      return 47#"Low_Mid_Tom"
    elif pitch in (43,58):
      return 43#"High_Floor_Tom"
    elif pitch in (26,46):
      return 46#"Open_Hi_Hat"
    elif pitch in (22,42,44):
      return 42#"Closed_Hi_Hat"
    elif pitch in (49,52,55,57):
      return 49#"Crash_Cymbal"
    elif pitch in (51,53,59):
      return 51#"Ride_Cymbal"
      
  def set_class(self,id):
    classes={"Bass":36,
        "Snare":38,
        "High_Tom":50,
        "Low_Mid_Tom":47,
        "High_Floor_Tom":43,
        "Open_Hi_Hat":46,
        "Closed_Hi_Hat":42,
        "Crash_Cymbal":49,
        "Ride_Cymbal":51}
    return [k for k,v in classes.items() if v == id][0]
  '''
  def tick2second(tick, ticks_per_beat, tempo):
    """Convert absolute time in ticks to seconds.
    Returns absolute time in seconds for a chosen MIDI file time
    resolution (ticks per beat, also called PPQN or pulses per quarter
    note) and tempo (microseconds per beat).
    """
    scale = tempo * 1e-6 / ticks_per_beat
    return tick * scale
  '''

class Midifile:
  def __init__(self,filepath):
    self.midifile=mido.MidiFile(filepath)
    self.PPQ=self.midifile.ticks_per_beat
    self.notes=[]
    
    self.parse_midi_messages()
    

    #self.total_ticks=self.set_length_ticks()
    #self.ending_tick=0

   #not explicitly required yet
    #self.key=''
    #self.clocks_per_click=0
    #self.notated_32nd_notes_per_beat=0
  
  def parse_midi_messages(self):

    elapsed_ticks=0
    for m,message in enumerate(self.midifile.tracks[0]):
      message_type=message.type
      
      elapsed_ticks+=message.time
      print(f'message {m} , time= {message.time} elapsed_ticks={elapsed_ticks}')

      if message_type=="set_tempo":
        self.tempo=message.tempo
        self.bpm=int(mido.tempo2bpm(self.tempo))

      elif message_type=="time_signature":
        self.rythm_numerator=message.numerator
        self.rythm_denominator=message.denominator
        self.clocks_per_click=message.clocks_per_click
        self.notated_32nd_notes_per_beat=message.notated_32nd_notes_per_beat
        
      elif message_type=="key_signature":
        self.key=message.key
      
      elif message_type=="end_of_track":
        self.ending_tick=message.time
        self.total_ticks=elapsed_ticks

      elif message_type=="note_on":
        channel=message.channel 
        pitch=message.note
        velocity=message.velocity
        time=message.time
        #duration=self.midifile.tracks[0][m+1].time
        #print(f'duration={self.midifile.tracks[0][m+1].time}')
        timestamp=elapsed_ticks
        timestamp_sec=self.convert_tic2sec(timestamp)
        if velocity!=0:
          temp_counter=1
          duration=0
          for msg in self.midifile.tracks[0][m+1:]:
            duration+=msg.time
            if msg.type=="note_on" and msg.note==pitch and msg.velocity==0:
              new_note=Note(channel,pitch,velocity,timestamp,timestamp_sec,duration,self.convert_tic2sec(duration))
              self.notes.append(new_note)
              break
      else:
        #print(f"Unknown message {message}")
        pass

  def get_midifile(self):
    return self.midifile
  def get_tempo(self):
    return self.tempo
  def get_bpm(self):
    return self.bpm
  def get_rythm_numerator(self):
    return self.rythm_numerator
  def get_rythm_denominator(self):
    return self.rythm_denominator
  def get_note(self,idx):
    return self.notes[idx]
  def get_num_notes(self):
    return len(self.notes)
  def get_total_ticks(self):
    return self.total_ticks
  def get_total_seconds(self):
    return mido.tick2second(self.total_ticks,self.PPQ,self.tempo)
  def get_ms_per_tick(self):
    return (60000 / (self.bpm * self.PPQ))
  def convert_tic2sec(self,tick):
    return mido.tick2second(tick,self.PPQ,self.tempo)

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
