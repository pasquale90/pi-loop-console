import mido 
from data.note import Note

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
    for mdt in range(len(self.midifile.tracks)):
      for m,message in enumerate(self.midifile.tracks[mdt]):
        
        message_type=message.type
        elapsed_ticks+=message.time
        #print(message, elapsed_ticks)
        #print(f'message {m} , time= {message.time} elapsed_ticks={elapsed_ticks}')

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
          #print(pitch, timestamp_sec)
          if velocity!=0:

            temp_counter=1
            duration=0
            for msg in self.midifile.tracks[mdt][m+1:]:
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
