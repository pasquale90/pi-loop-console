#from paths import *
import paths
import midifile,audio,utils,waveform
import os
import mido #import MidiFile, tempo2bpm, tick2second
import numpy as np
import pandas as pd
#import librosa
from tqdm import tqdm
import math

def create_segments(csvpath,audio_sr=2000,numMeasures=4): 
  csv=pd.read_csv(csvpath)
  print(f'csv of shape \t {len(csv.index.values)} x {len(csv.columns.values)}')
  print(f'rows {csv.index.values}')
  print(f'columns {csv.columns.values}')

  #keep
  audiosegments,midisegments=[],[]
  midi_velocities=[]
  
  #for each file
  for i in tqdm(range(csv.shape[0])):
    #get the filepath
    audio_relpath=csv['audio_filename'].loc[csv.index[i]]
    midi_relpath=csv['midi_filename'].loc[csv.index[i]]
    print(f"audiofile {audio_relpath}")
    print(f"midifile {midi_relpath}")
    
    #read the file - audio and midi
    audiotrack=audio.Audio(paths.get_abs_datapath(audio_relpath),sampling_rate=audio_sr)
    miditrack=midifile.Midifile(paths.get_abs_datapath(midi_relpath))    
    Segment(miditrack,audiotrack,numMeasures)
    
    break
    
  audiosegments,midisegments,midi_velocities=[],[],[]
  return audiosegments,midisegments,midi_velocities


class Segment:
  def __init__(self,miditrack,audiotrack,measures_per_segment=4,quantization=16):
    self.audiosegments,self.midisegments=[],[]
    self.midi_velocities=[]

    self.temp_create_segment(miditrack,audiotrack,measures_per_segment,quantization)

  def get_segment_length_secs(self,measures_per_segment,miditrack):
      segment_length_secs=measures_per_segment*(miditrack.get_rythm_numerator()*miditrack.get_quarter_length_secs())
      print(f"segment_length_secs {segment_length_secs}")
      segment_length_ticks=miditrack.convert_sec2tick (segment_length_secs)
      print(f"segment_length_ticks {segment_length_ticks}")
      return segment_length_secs

  def get_segments_avail(self,miditrack,audiotrack,segment_length_secs):
    avail_seconds=min(miditrack.get_total_seconds(),audiotrack.get_duration_sec())
    print(f"avail_seconds {avail_seconds}")
    avail_segments=math.floor(avail_seconds/segment_length_secs)
    print(f"avail_segments {avail_segments}")
    return avail_segments  
  def get_PPQuantizationUnit(self,PPQ,quantization):
    return int(PPQ/(quantization/4))
  def quantize(self,value,PPQU):
    return value//PPQU
  def temp_create_segment(self,miditrack,audiotrack,measures_per_segment,quantization): 

    # read audio
    audio_sr=audiotrack.get_sampling_rate()
    audio_duration=audiotrack.get_duration_sec()
    raw=audiotrack.get_raw()
    # print(f"audio_sr {audio_sr}")
    # print(f"audio_duration {audio_duration}")
    
    """
    #read midi
    midi_tempo=miditrack.get_tempo()
    midi_bpm=miditrack.get_bpm()
    midi_numer=miditrack.get_rythm_numerator()
    midi_denom=miditrack.get_rythm_denominator()
    midi_note0=miditrack.get_note(0)
    midi_numnotes=miditrack.get_num_notes()
    midi_totalticks=miditrack.get_total_ticks()
    midi_totalseconds=miditrack.get_total_seconds()
    midi_mspertick=miditrack.get_ms_per_tick()
    midi_convert_tick_to_second=miditrack.convert_tic2sec(512)
    midi_ppq=miditrack.get_ppq()
    midi_quarter_length_secs=miditrack.get_quarter_length_secs()
    print("midi_totalseconds ",midi_totalseconds)
    print("quarter length seconds ",miditrack.get_quarter_length_secs())
    print("quarter lenth ticks ",miditrack.get_quarter_length_ticks())
    print("convert_tic2sec(480)*midi.get_rythm_numerator() ",miditrack.convert_tic2sec(480)*miditrack.get_rythm_numerator())
    print("get_whole_measure_length_ticks ",miditrack.get_whole_measure_length_ticks())
    print("get_whole_measure_length_secs ",miditrack.get_whole_measure_length_secs())
    """

    # calculate how many seconds per segment
    segment_length_secs=self.get_segment_length_secs(measures_per_segment,miditrack)

    # how many segments can be produced by each file
    avail_segments=self.get_segments_avail(miditrack,audiotrack,segment_length_secs)

    #crop audio
    ## calc audio samples
    audiosamples_per_segment=int(segment_length_secs*audio_sr)
    # print(f"audiosamples_per_segment {audiosamples_per_segment}")
    
    ## create segments
    for seg_step in range(avail_segments):
      #midi_segment_length=midi.convert_sec2tick(segment_length_secs)
      #midisegment=np.zeros(int(midi_segment_length), dtype=int)
      
      # print(f'SegStep {seg_step}.Appending samples from {segment_length_secs*seg_step} : {segment_length_secs*(seg_step+1)} secs')
      print(f'SegStep {seg_step}.Appending samples from {miditrack.convert_sec2tick(segment_length_secs*seg_step)} : {miditrack.convert_sec2tick(segment_length_secs*(seg_step+1))} ticks')
      # print("\tppq = ",miditrack.get_ppq()," TOTAL ticks for 2 measures = ",miditrack.get_rythm_numerator()*miditrack.get_ppq()*measures_per_segment)
      ### audio samples
      from_audiosample=seg_step*audiosamples_per_segment
      upto_audiosample=(seg_step+1)*audiosamples_per_segment
      # print(f"from_audiosample {from_audiosample}")
      # print(f"upto_audiosample {upto_audiosample}")
      audiosegment=raw[from_audiosample:upto_audiosample]
      # print(f"audio segment length {len(audiosegment)}")
      #break in windows -- for feeding the model
      self.audiosegments.append(audiosegment)

      ### midi samples
      temp_midivelocities=[]

      midisegment=np.zeros(measures_per_segment*quantization)
      for j in range(miditrack.get_num_notes()):
        midi_note=miditrack.get_note(j)

        timestamp_secs=midi_note.get_timestamp_sec()
        
        timestamp=midi_note.get_timestamp()
        if timestamp>=miditrack.convert_sec2tick(segment_length_secs*seg_step) and \
           timestamp<miditrack.convert_sec2tick(segment_length_secs*(seg_step+1)):

        # if segment_length_secs*seg_step<timestamp_secs and segment_length_secs*(seg_step+1)>timestamp_secs:
        #   print(f'sec to tick {miditrack.convert_sec2tick(segment_length_secs*seg_step)} to {miditrack.convert_sec2tick(segment_length_secs*(seg_step+1))}, timestamp_secs to ticks {miditrack.convert_sec2tick(timestamp_secs)}')
        #   print(f'sec {segment_length_secs*seg_step} to {segment_length_secs*(seg_step+1)}, timestamp_secs {timestamp_secs}')
          pitch, timestamp, duration, velocity = midi_note.get_pitch(), midi_note.get_timestamp(), midi_note.get_duration(), midi_note.get_velocity()
          print(f'\tpitch {pitch},timestamp {timestamp},duration {duration},velocity {velocity}')
          PPQuantizationUnit=self.get_PPQuantizationUnit(miditrack.get_ppq(),quantization)
          print(PPQuantizationUnit)
          Quantization_position=self.quantize(timestamp,PPQuantizationUnit)
          print(f'\ttimestamp {timestamp} --> Quantization_position {Quantization_position}')
          quantized_duration=self.quantize(duration,PPQuantizationUnit)
          print(f'\tduration {duration} --> quantized_duration {quantized_duration}')
          

          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          # ensure quantization is implemented correctly
          if j>20:
            break
          
      # break
  """        
          #post process timestamps to align them to a new value according to the segment's timestamp
          timestamp=timestamp-midi_segment_length*seg_step
          
          # create an encoded array of type : i.e. assuming duration 3 --> [0 0 0 0 0 pitch pitch pitch 0 0 0 0]
          midisegment[timestamp:timestamp+duration]=pitch
          temp_midivelocities.append(velocity)
      #midilabels.append([timestamp,midi_note.get_label(),midi_note.get_duration_sec(),midi_note.get_velocity()])
      midisegments.append(midisegment)
      midi_velocities.append(temp_midivelocities)
    
    if i>10:
      break
  return audiosegments,midisegments,midi_velocities
  """