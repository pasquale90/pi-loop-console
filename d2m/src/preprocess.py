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

def create_segments(csvpath,audio_sr,numMeasures,quantization=4): 
  csv=pd.read_csv(csvpath)
  print(f'csv of shape \t {len(csv.index.values)} x {len(csv.columns.values)}')
  print(f'rows {csv.index.values}')
  print(f'columns {csv.columns.values}')

  
  audiosegments,midisegments=[],[]
  
  #for each file
  for i in tqdm(range(csv.shape[0])):
    #get the filepath
    audio_relpath=csv['audio_filename'].loc[csv.index[i]]
    midi_relpath=csv['midi_filename'].loc[csv.index[i]]
    # print(f"audiofile {audio_relpath}")
    # print(f"midifile {midi_relpath}")
    
    #read the file - audio and midi
    audiotrack=audio.Audio(paths.get_abs_datapath(audio_relpath),sampling_rate=audio_sr)
    miditrack=midifile.Midifile(paths.get_abs_datapath(midi_relpath))    

    segment=Segment(miditrack,audiotrack)
    new_audiosegments,new_midisegments=segment.produce_segments(measures_per_segment=numMeasures,quantization=quantization)

    audiosegments.extend(new_audiosegments)
    midisegments.extend(new_midisegments)
    break
  return audiosegments,midisegments


class Segment:
  def __init__(self,miditrack,audiotrack):
    

    self.miditrack=miditrack
    self.audiotrack=audiotrack

    self.num_classes=self.miditrack.get_note(0).get_numclasses()
    
    self.audiosegments,self.midisegments=[],[]

    # self.temp_create_segment(miditrack,audiotrack,measures_per_segment)
      
  def _get_segment_length_secs(self,measures_per_segment):
      segment_length_secs=measures_per_segment*(self.miditrack.get_rythm_numerator()*self.miditrack.get_quarter_length_secs())
      # print(f"segment_length_secs {segment_length_secs}")
      segment_length_ticks=self.miditrack.convert_sec2tick (segment_length_secs)
      # print(f"segment_length_ticks {segment_length_ticks}")
      return segment_length_secs
  
  def _get_segment_length_ticks(self,measures_per_segment):
      segment_length_ticks=measures_per_segment*(self.miditrack.get_rythm_numerator()*self.miditrack.get_ppq())
      # print(f"__get_segment_length_ticks:: segment_length_ticks {segment_length_ticks}")
      return segment_length_ticks

  def _get_segments_avail(self,segment_length_secs):
    avail_seconds=min(self.miditrack.get_total_seconds(),self.audiotrack.get_duration_sec())
    # print(f"avail_seconds {avail_seconds}")
    avail_segments=math.floor(avail_seconds/segment_length_secs)
    # print(f"avail_segments {avail_segments}")
    return avail_segments  

  """
  # Normalization method
  # calc quantization units - how many units fit in a measure  
  # i.e how many 16nth notes fit to a measure divided in pieces of 4 (that is a {whatever}/4 measure)
  def _calc_quantization_of_units(self,denominator):
    return int(self.quantization/denominator)
  """
  # calc how many quantization units exist in a measure that has {numerator} units and is divided by {denominator} units
  # i.e how many 16nth notes has a measure that has a 3/4.
  def _get_units_per_measure(self,quantization,numerator,denominator):
      # print("numerator ",numerator,"denominator ",denominator)
      return int(numerator*(quantization/denominator))

  # calc how many midi-ticks per qunatization unit 
  # i.e. each 64nth note in a midifile with ppq=480 , has a resolution of 480/(64/4)=30 ticks
  def _get_TicksPerQuantizationUnit(self,quantization,PPQ):
    units_per_quarter=quantization/4 # by 4 since, ppq defines pulses per quarter(=4)
    return int(PPQ/(units_per_quarter))

  # decide in which of the quantization units, a note approximately exists
  def quantize(self,value,TPQU):
    return round(value/TPQU)

  def produce_segments(self,measures_per_segment,quantization): 

    # read audio
    audio_sr=self.audiotrack.get_sampling_rate()
    audio_duration=self.audiotrack.get_duration_sec()
    raw=self.audiotrack.get_raw()
    # print(f"audio_sr {audio_sr}")
    
    # calculate corresponding values per segment
    segment_length_secs=self._get_segment_length_secs(measures_per_segment)
    ## calc audio samples per segment
    audiosamples_per_segment=int(segment_length_secs*audio_sr)
    ## calc ticks per segment
    miditicks_per_segment=self._get_segment_length_ticks(measures_per_segment)

    # how many segments can be produced by each file
    avail_segments=self._get_segments_avail(segment_length_secs)


    ## calculate the size of the "quantization" units
    TPQU=self._get_TicksPerQuantizationUnit(quantization,self.miditrack.get_ppq())
    # print("TPQU ",TPQU)
    
    ## create segments
    for seg_step in range(avail_segments):
      # audio samples window
      from_audiosample=seg_step*audiosamples_per_segment
      upto_audiosample=(seg_step+1)*audiosamples_per_segment

      # midi samples window
      from_miditick=seg_step*miditicks_per_segment
      upto_miditick=(seg_step+1)*miditicks_per_segment
      # print(f'"\n\nSegStep {seg_step}.From second {segment_length_secs*seg_step} up to {segment_length_secs*(seg_step+1)}. From audiosamle {from_audiosample} up to {upto_audiosample}. From miditick {from_miditick} up to {upto_miditick}')
      
      # get audio segment
      audiosegment=raw[from_audiosample:upto_audiosample]
      # print(f"audio segment length {len(audiosegment)}")
      self.audiosegments.append(audiosegment)

      # get midi segment
      ## define resolution (quantization) --> @numzeros=measures_per_segment * quantization * quantization notes fit in a midi_numerator fit in a sing measure
      units_per_measure=self._get_units_per_measure(quantization,self.miditrack.get_rythm_numerator(),self.miditrack.get_rythm_denominator())
      # print("units_per_measure ",units_per_measure)
      numzeros=measures_per_segment*units_per_measure
      # print(f'\tnumzeros {numzeros}')
      #create a zero-value 2D array of dim [classes,numzeros]
      midisegment=np.zeros((self.num_classes,numzeros), dtype=np.int)      
      # print("num notes in midi =",self.miditrack.get_num_notes())
            
      for j in range(self.miditrack.get_num_notes()):
        midi_note=self.miditrack.get_note(j)
        timestamp=midi_note.get_timestamp()
        if timestamp>=from_miditick and \
            timestamp<upto_miditick:

          #shift_note_timestamp
          time_shift=timestamp-from_miditick
          timestamp , _ = self.miditrack.shift_note_timestamp(j,time_shift)          
          
          # get label id
          label_id=midi_note.get_label_id()
          
          # get velocity
          velocity=midi_note.get_velocity()

          # get timestamp
          duration=midi_note.get_duration()
          
          qtimestamp=self.quantize(timestamp,TPQU)
          qduration=self.quantize(duration,TPQU)
          # if timestamp+duration>=upto_miditick-from_miditick:
          #   duration=upto_miditick-from_miditick-timestamp

          
          # print(f'\tnote: {j}, timestamp {timestamp}, duration {duration}, label_id {label_id}, velocity {velocity}')
          # print(f'\tqtimestamp {qtimestamp}, qduration {qduration}\n')
          

          # form the final midisegment label 
          midisegment[label_id][qtimestamp:qtimestamp+qduration]=velocity
  
      self.midisegments.append(midisegment)
      
    return self.audiosegments,self.midisegments
  