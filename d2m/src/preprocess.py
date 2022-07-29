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

def create_segments(csvpath,audio_sr=2000,numQuarters=4,temp_print=False): 
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
    audiotrack=audio.Audio(paths.get_abs_datapath(audio_relpath),audio_sr)
    miditrack=midifile.Midifile(paths.get_abs_datapath(midi_relpath))    
    Segment(miditrack,audiotrack,numQuarters)
    
    
    
    
    
    break
    """
    ## SKILL POINT
    midi=miditrack
    
    midi_tempo=midi.get_tempo()
    midi_bpm=midi.get_bpm()
    midi_numer=midi.get_rythm_numerator()
    midi_denom=midi.get_rythm_denominator()
    midi_note0=midi.get_note(0)
    midi_numnotes=midi.get_num_notes()
    midi_totalticks=midi.get_total_ticks()
    midi_totalseconds=midi.get_total_seconds()
    midi_mspertick=midi.get_ms_per_tick()
    
    print("quarter length seconds ",midi.get_quarter_length_secs())
    print("quarter lenth ticks ",midi.get_quarter_length_ticks())
    print("convert_tic2sec(480)*midi.get_rythm_numerator() ",midi.convert_tic2sec(480)*midi.get_rythm_numerator())

    print("get_whole_measure_length_ticks ",midi.get_whole_measure_length_ticks())
    print("get_whole_measure_length_secs ",midi.get_whole_measure_length_secs())
    

    midi_convert_tick_to_second=midi.convert_tic2sec(512)
    midi_ppq=midi.get_ppq()
    
    # read audio
    audio_sr=aud.get_sampling_rate()
    audio_ismono=aud.is_mono()
    audio_duration=aud.get_duration_sec()
    
    if temp_print:
      print(f"audio_sr {audio_sr}")
      print(f"audio_ismono {audio_ismono}")
      print(f"audio_duration {audio_duration}")
 
  
    #working on segments --> pass them on segment class
    quarter_length_secs=midi_ppq*midi_mspertick*midi_numer/1000
    if temp_print:
      print(f"quarter_length_secs {quarter_length_secs}")
    #we need 4 meters(which are 4 quarters) to create a segment
    segment_length_secs=4*quarter_length_secs
    if temp_print:
      print(f"segment_length_secs {segment_length_secs}")
    
    # how many segments can be produced by each file
    avail_seconds=min(midi_totalseconds,audio_duration)
    if temp_print:
      print(f"avail_seconds {avail_seconds}")
    avail_segments=math.floor(avail_seconds/segment_length_secs)
    if temp_print:
      print(f"avail_segments {avail_segments}")
    #crop audio
    ## calc audio samples
    audiosamples_per_segment=int(segment_length_secs*audio_sr)
    if temp_print:
      print(f"audiosamples_per_segment {audiosamples_per_segment}")

    ## create segments
    for seg_step in range(avail_segments):
      midi_segment_length=midi.convert_sec2tick(segment_length_secs)
      midisegment=np.zeros(int(midi_segment_length), dtype=int)
      
      if temp_print:
        print(f'Appending samples from {segment_length_secs*seg_step} : {segment_length_secs*(seg_step+1)}')

      ### audio samples
      from_audiosample=seg_step*audiosamples_per_segment
      upto_audiosample=(seg_step+1)*audiosamples_per_segment
      if temp_print:
        print(f"from_audiosample {from_audiosample}")
        print(f"upto_audiosample {upto_audiosample}")
      segment=raw[from_audiosample:upto_audiosample]
      #break in windows -- for feeding the model
      audiosegments.append(segment)

      ### midi samples (pending...)
      if temp_print:
        print("MIDIFILEEEEEEEEEEEEEEEEEEEEEEEE")
        
      temp_midivelocities=[]
      for j in range(midi_numnotes):
        midi_note=midi.get_note(j)
        timestamp_secs=midi_note.get_timestamp_sec()
        
        
        if segment_length_secs*seg_step<timestamp_secs and segment_length_secs*(seg_step+1)>timestamp_secs:
          if temp_print:
            print(f"midinote {j} has timestamp {timestamp_secs}")
      
            print(f'note-label {midi_note.get_label()}, duration  {midi_note.get_duration_sec()}, velocity {midi_note.get_velocity()}')
          
          
          note=midi.get_note(j)
          pitch, timestamp, duration, velocity = note.get_pitch(), note.get_timestamp(), note.get_duration(), note.get_velocity()
          #print(idx, pitch, timestamp, duration, velocity )
          
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
    """
    
  audiosegments,midisegments,midi_velocities=[],[],[]
  return audiosegments,midisegments,midi_velocities


class Segment:
  def __init__(self,miditrack,audiotrack,measures_per_segment=4):
    self.audiosegments,self.midisegments=[],[]
    self.midi_velocities=[]

    self.temp_create_segment(miditrack,audiotrack,measures_per_segment)

  def temp_create_segment(self,miditrack,audiotrack,measures_per_segment): 
    
    raw=audiotrack.get_raw()
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
    print("quarter length seconds ",miditrack.get_quarter_length_secs())
    print("quarter lenth ticks ",miditrack.get_quarter_length_ticks())
    print("convert_tic2sec(480)*midi.get_rythm_numerator() ",miditrack.convert_tic2sec(480)*miditrack.get_rythm_numerator())
    print("get_whole_measure_length_ticks ",miditrack.get_whole_measure_length_ticks())
    print("get_whole_measure_length_secs ",miditrack.get_whole_measure_length_secs())

    #we need 4 meters(which are 4 quarters) to create a segment
    segment_length_secs=measures_per_segment*(midi_numer*midi_quarter_length_secs)
    print(f"segment_length_secs {segment_length_secs}")
    segment_length_ticks=miditrack.second2tick(segment_length_secs)
    print(f"segment_length_ticks {segment_length_ticks}")
    """
    # read audio
    audio_sr=audiotrack.get_sampling_rate()
    audio_ismono=audiotrack.is_mono()
    audio_duration=audiotrack.get_duration_sec()
    print(f"audio_sr {audio_sr}")
    print(f"audio_ismono {audio_ismono}")
    print(f"audio_duration {audio_duration}")
    

    
    # how many segments can be produced by each file
    avail_seconds=min(midi_totalseconds,audio_duration)
    print(f"avail_seconds {avail_seconds}")
    avail_segments=math.floor(avail_seconds/segment_length_secs)
    print(f"avail_segments {avail_segments}")
    #crop audio
    ## calc audio samples
    audiosamples_per_segment=int(segment_length_secs*audio_sr)
    print(f"audiosamples_per_segment {audiosamples_per_segment}")
    """

  """
    
    ## create segments
    for seg_step in range(avail_segments):
      midi_segment_length=midi.convert_sec2tick(segment_length_secs)
      midisegment=np.zeros(int(midi_segment_length), dtype=int)
      
      if temp_print:
        print(f'Appending samples from {segment_length_secs*seg_step} : {segment_length_secs*(seg_step+1)}')

      ### audio samples
      from_audiosample=seg_step*audiosamples_per_segment
      upto_audiosample=(seg_step+1)*audiosamples_per_segment
      if temp_print:
        print(f"from_audiosample {from_audiosample}")
        print(f"upto_audiosample {upto_audiosample}")
      segment=raw[from_audiosample:upto_audiosample]
      #break in windows -- for feeding the model
      audiosegments.append(segment)

      ### midi samples (pending...)
      if temp_print:
        print("MIDIFILEEEEEEEEEEEEEEEEEEEEEEEE")
        
      temp_midivelocities=[]
      for j in range(midi_numnotes):
        midi_note=midi.get_note(j)
        timestamp_secs=midi_note.get_timestamp_sec()
        
        
        if segment_length_secs*seg_step<timestamp_secs and segment_length_secs*(seg_step+1)>timestamp_secs:
          if temp_print:
            print(f"midinote {j} has timestamp {timestamp_secs}")
      
            print(f'note-label {midi_note.get_label()}, duration  {midi_note.get_duration_sec()}, velocity {midi_note.get_velocity()}')
          
          
          note=midi.get_note(j)
          pitch, timestamp, duration, velocity = note.get_pitch(), note.get_timestamp(), note.get_duration(), note.get_velocity()
          #print(idx, pitch, timestamp, duration, velocity )
          
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











































































'''
def create_segments(csv,temp_print=False): 
  
  #keep
  audiosegments,midisegments=[],[]
  midilabels=[]
  midi_velocities=[]
  
  #erase
  #ml_notes,ml_durations,ml_velocities=[],[],[]
  
  
  #for each file
  for i in tqdm(range (csv.shape[0])):
    #get the filepath
    midi_relpath=csv['midi_filename'].loc[csv.index[i]]
    audio_relpath=csv['audio_filename'].loc[csv.index[i]]
    print(f"file {midi_relpath}")
    #read the file - audio and midi
    midi=midifile.Midifile(get_abs_datapath(midi_relpath))
    aud=audio.Audio(get_abs_datapath(audio_relpath),2000)
    raw=aud.get_raw()
    
    #read midi
    midi_tempo=midi.get_tempo()
    midi_bpm=midi.get_bpm()
    midi_numer=midi.get_rythm_numerator()
    midi_denom=midi.get_rythm_denominator()
    midi_note0=midi.get_note(0)
    midi_numnotes=midi.get_num_notes()
    midi_totalticks=midi.get_total_ticks()
    midi_totalseconds=midi.get_total_seconds()
    midi_mspertick=midi.get_ms_per_tick()
    midi_convert_tick_to_second=midi.convert_tic2sec(512)
    midi_ppq=midi.get_ppq()
    
 
    audio_sr=aud.get_sampling_rate()
    audio_ismono=aud.is_mono()
    audio_duration=aud.get_duration_sec()
    
    if temp_print:
      print(f"audio_sr {audio_sr}")
      print(f"audio_ismono {audio_ismono}")
      print(f"audio_duration {audio_duration}")
 
 
 
 
    
    #working on segments --> pass them on segment class
    quarter_length_secs=midi_ppq*midi_mspertick*midi_numer/1000
    if temp_print:
      print(f"quarter_length_secs {quarter_length_secs}")
    #we need 4 meters(which are 4 quarters) to create a segment
    segment_length_secs=4*quarter_length_secs
    if temp_print:
      print(f"segment_length_secs {segment_length_secs}")
    
    # how many segments can be produced by each file
    avail_seconds=min(midi_totalseconds,audio_duration)
    if temp_print:
      print(f"avail_seconds {avail_seconds}")
    avail_segments=math.floor(avail_seconds/segment_length_secs)
    if temp_print:
      print(f"avail_segments {avail_segments}")
    #crop audio
    ## calc audio samples
    audiosamples_per_segment=int(segment_length_secs*audio_sr)
    if temp_print:
      print(f"audiosamples_per_segment {audiosamples_per_segment}")

    ## create segments
    for seg_step in range(avail_segments):
      midi_segment_length=midi.convert_sec2tick(segment_length_secs)
      midisegment=np.zeros(int(midi_segment_length), dtype=int)
      
      if temp_print:
        print(f'Appending samples from {segment_length_secs*seg_step} : {segment_length_secs*(seg_step+1)}')

      ### audio samples
      from_audiosample=seg_step*audiosamples_per_segment
      upto_audiosample=(seg_step+1)*audiosamples_per_segment
      if temp_print:
        print(f"from_audiosample {from_audiosample}")
        print(f"upto_audiosample {upto_audiosample}")
      segment=raw[from_audiosample:upto_audiosample]
      #break in windows -- for feeding the model
      audiosegments.append(segment)

      ### midi samples (pending...)
      if temp_print:
        print("MIDIFILEEEEEEEEEEEEEEEEEEEEEEEE")
        
      temp_midivelocities=[]
      for j in range(midi_numnotes):
        midi_note=midi.get_note(j)
        timestamp_secs=midi_note.get_timestamp_sec()
        
        
        if segment_length_secs*seg_step<timestamp_secs and segment_length_secs*(seg_step+1)>timestamp_secs:
          if temp_print:
            print(f"midinote {j} has timestamp {timestamp_secs}")
      
            print(f'note-label {midi_note.get_label()}, duration  {midi_note.get_duration_sec()}, velocity {midi_note.get_velocity()}')
          
          
          note=midi.get_note(j)
          pitch, timestamp, duration, velocity = note.get_pitch(), note.get_timestamp(), note.get_duration(), note.get_velocity()
          #print(idx, pitch, timestamp, duration, velocity )
          
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
'''  

'''
def create_segments(csv,temp_print=False): 
  audiosegments=[]
  midilabels=[]
  temp_timestamps=[]
  
  ml_notes,ml_durations,ml_velocities=[],[],[]
  for i in tqdm(range (csv.shape[0])):
    midi_relpath=csv['midi_filename'].loc[csv.index[i]]
    audio_relpath=csv['audio_filename'].loc[csv.index[i]]

    midi=midifile.Midifile(get_abs_datapath(midi_relpath))
    aud=audio.Audio(get_abs_datapath(audio_relpath),2000)
    raw=aud.get_raw()

    midi_tempo=midi.get_tempo()
    midi_bpm=midi.get_bpm()
    midi_numer=midi.get_rythm_numerator()
    midi_denom=midi.get_rythm_denominator()
    midi_note0=midi.get_note(0)
    midi_numnotes=midi.get_num_notes()
    midi_totalticks=midi.get_total_ticks()
    midi_totalseconds=midi.get_total_seconds()
    midi_mspertick=midi.get_ms_per_tick()
    midi_convert_tick_to_second=midi.convert_tic2sec(512)
    midi_ppq=midi.get_ppq()
    
    if temp_print:
      print(f"#### midi_relpath #### \n{midi_relpath}")
      print(f"midi_tempo {midi_tempo}")
      print(f"midi_bpm {midi_bpm}")
      print(f"midi_numer {midi_numer}")
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
    
    if temp_print:
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

    #working on segments --> pass them on segment class
    quarter_length_secs=midi_ppq*midi_mspertick*midi_numer/1000
    if temp_print:
      print(f"quarter_length_secs {quarter_length_secs}")
    #we need 4 meters(which are 4 quarters) to create a segment
    segment_length_secs=4*quarter_length_secs
    if temp_print:
      print(f"segment_length_secs {segment_length_secs}")
    
    # how many segments can be produced by each file
    avail_seconds=min(midi_totalseconds,audio_duration)
    if temp_print:
      print(f"avail_seconds {avail_seconds}")
    avail_segments=math.floor(avail_seconds/segment_length_secs)
    if temp_print:
      print(f"avail_segments {avail_segments}")
    #crop audio
    ## calc audio samples
    audiosamples_per_segment=int(segment_length_secs*audio_sr)
    if temp_print:
      print(f"audiosamples_per_segment {audiosamples_per_segment}")

    ## create segments
    for seg_step in range(avail_segments):
      
      if temp_print:
        print(f'Appending samples from {segment_length_secs*seg_step} : {segment_length_secs*(seg_step+1)}')

      ### audio samples
      from_audiosample=seg_step*audiosamples_per_segment
      upto_audiosample=(seg_step+1)*audiosamples_per_segment
      if temp_print:
        print(f"from_audiosample {from_audiosample}")
        print(f"upto_audiosample {upto_audiosample}")
      segment=raw[from_audiosample:upto_audiosample]
      #break in windows -- for feeding the model
      audiosegments.append(segment)

      ### midi samples (pending...)
      if temp_print:
        print("MIDIFILEEEEEEEEEEEEEEEEEEEEEEEE")
      for j in range(midi_numnotes):
        midi_note=midi.get_note(j)
        timestamp=midi_note.get_timestamp_sec()
        if segment_length_secs*seg_step<timestamp and segment_length_secs*(seg_step+1)>timestamp:
          if temp_print:
            print(f"midinote {j} has timestamp {timestamp}")
      
            print(f'note-label {midi_note.get_label()}, duration  {midi_note.get_duration_sec()}, velocity {midi_note.get_velocity()}')
          
          #midilabels.append([timestamp,midi_note.get_label(),midi_note.get_duration_sec(),midi_note.get_velocity()])
          temp_timestamps.append(timestamp)
          break


    if i>10:
      break

  #audiofiles, midifiles=utils.pathscan(len(folders))
  #print(len(audiofiles), len(midifiles))
  
  
  #return audiosegments, midilabels
  return audiosegments, temp_timestamps #temporary




def audio2frames(self):
  frames = []
  #get windows out of the raw waveform
  for count_frames,(start,end) in enumerate(_windows(raw,window_size,hop_length)):
    if(len(raw[start:end]) == window_size):
      #print(start,end)
      frame = raw[start:end]#rectangular window


#define analysis parameters
def analysis_parameters():
    sampling_rate=16000  #for instance: if mode 16, sr = 16kHz
    window_size = sampling_rate
    overlap = int(window_size*0.75)
    hop_length = (window_size-overlap)
    return sampling_rate, window_size, hop_length


def _windows(data, window_size, hop_length):
    start = 0
    while start < len(data):
        yield start, start + window_size
        start += hop_length
                    
#sampling_rate=16000
raw, sr =librosa.load(audiosample, sr=sampling_rate, mono=True)
print(raw.shape)

frames = []
#get windows out of the raw waveform
for count_frames,(start,end) in enumerate(_windows(raw,window_size,hop_length)):
if(len(raw[start:end]) == window_size):
    print(start,end)
    frame = raw[start:end]#rectangular window


def preprocess_data(audio_path, folds, files, mode):
    
  def _windows(data, window_size, hop_length):
    start = 0
    while start < len(data):
      yield start, start + window_size
      start += hop_length

  def normalize(data):#in the space [-1,1]
    #return (data - np.min(data)) / (np.max(data) - np.min(data))#[0,1]
    return (2*(data-np.min(data))/(np.max(data) - np.min(data)))-1
  

  features = []
 
  extr = True
  if extr == True:
    
    print('Preprocessing data ........ ')
    
    sampling_rate, window_size, hop_length = analysis_parameters(mode)
    print(f'sampling_rate: {sampling_rate}, window_size: {window_size}, hop_length: {hop_length}')
0
    shape_print=True


    for count_files, file in tqdm(enumerate(audiofiles)):
      name = file.split('.wav')[0]

      #readfile
      raw, sr =librosa.load(audio_path+file, sr=sampling_rate, mono=True)
      #print(f'{file} had length {len(raw)}')

      #normalize
      raw = normalize(raw)
      
      frames = []
      #get windows out of the raw waveform
      for count_frames,(start,end) in enumerate(_windows(raw,window_size,hop_length)):
        if(len(raw[start:end]) == window_size):
          #print(start,end)
          frame = raw[start:end]#rectangular window

          #flatten
          #implemented inside the model

          frames.append(frame)
      
      features.append(frames)
     

      if shape_print:
            print('\nFeature Shape Check\n')
            print(f'raw has len:{len(raw)/sampling_rate}')
            print(f'Postprocessed feature has shape : {np.asarray(features).shape} with min:{np.asarray(features[0]).min()} and max:{np.asarray(features[0]).max()}]')
            print()
            shape_print = False

  #"""
  print('len(features)-features',len(features))
  print('len(features[0])-freq_domain',len(features[0]))
  print('labels',len(labels))
  #"""
  return features

#get analysis parameters
sampling_rate, window_size, hop_length = analysis_parameters(mode)
print(f'sampling_rate: {sampling_rate}, window_size: {window_size}, hop_length: {hop_length}')

#extract_features       
features = preprocess_data(audio_path, folds, audiofiles, mode)
#features, labels, folders = extract_mel_spectogram(audio_path,us8k,folds,audiofiles,sampling_rate,hop_length,fft_points,mel_bands)
print(f' feature\'s len : {len(features)}, labels : {len(labels)}, folders : {len(folders)}')
'''
