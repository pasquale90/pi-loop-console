# ,numMeasures=hyperparams.numMeasures,quantization=hyperparams.quantization
import math
import numpy as np
import paths
import midifile,audio,utils,waveform





class Segment:
  def __init__(self,midi_relpath,audio_relpath,audio_sr,numMeasures,quantization):

    #read the file - audio and midi
    self.miditrack=midifile.Midifile(paths.get_abs_datapath(midi_relpath))
    self.audiotrack=audio.Audio(paths.get_abs_datapath(audio_relpath),sampling_rate=audio_sr)
    self.audio_sr=audio_sr
    self.measures_per_segment=numMeasures
    self.quantization=quantization
    
    # variable definition here - computational economy
    self.num_classes=self.miditrack.get_note(0).get_numclasses()
    
      
  def _calc_segment_length_secs(self):
      segment_length_secs=self.measures_per_segment*(self.miditrack.get_rythm_numerator()*self.miditrack.get_quarter_length_secs())
      # print(f"segment_length_secs {segment_length_secs}")
      return segment_length_secs

  def _calc_segment_length_ticks(self):
    segment_length_ticks=self.measures_per_segment*(self.miditrack.get_rythm_numerator()*self.miditrack.get_ppq())
    # could be also calculated as if ..
    # self.segment_length_ticks=self.miditrack.convert_sec2tick (segment_length_secs)
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
  def _get_units_per_measure(self,numerator,denominator):
      # print("numerator ",numerator,"denominator ",denominator)
      return int(numerator*(self.quantization/denominator))

  # calc how many midi-ticks per qunatization unit 
  # i.e. each 64nth note in a midifile with ppq=480 , has a resolution of 480/(64/4)=30 ticks
  def _get_TicksPerQuantizationUnit(self,PPQ):
    units_per_quarter=self.quantization/4 # by 4 since, ppq defines pulses per quarter(=4)
    return int(PPQ/(units_per_quarter))

  # decide in which of the quantization units, a note approximately exists
  def quantize(self,value,TPQU):
    return round(value/TPQU)

  def create_segments(self): 
    audio_segments,midi_segments=[],[]

    # read audio
    audio_sr=self.audiotrack.get_sampling_rate()
    audio_duration=self.audiotrack.get_duration_sec()
    raw=self.audiotrack.get_raw()
    # print(f"audio_sr {audio_sr}")
    
    # calculate corresponding values per segment
    segment_length_secs=self._calc_segment_length_secs()
    ## calc audio samples per segment
    audiosamples_per_segment=int(segment_length_secs*audio_sr)
    ## calc ticks per segment
    miditicks_per_segment=self._calc_segment_length_ticks()

    # how many segments can be produced by each file
    avail_segments=self._get_segments_avail(segment_length_secs)

    ## calculate the size of the "quantization" units
    TPQU=self._get_TicksPerQuantizationUnit(self.miditrack.get_ppq())
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
      audio_segment=raw[from_audiosample:upto_audiosample]
      # print(f"audio segment length {len(audiosegment)}")
      audio_segments.append(audio_segment)
      print(audio_segment.shape)

      # get midi segment
      ## define resolution (quantization) --> @numzeros=measures_per_segment * quantization * quantization notes fit in a midi_numerator fit in a sing measure
      units_per_measure=self._get_units_per_measure(self.miditrack.get_rythm_numerator(),self.miditrack.get_rythm_denominator())
      # print("units_per_measure ",units_per_measure)
      
      numzeros=self.measures_per_segment*units_per_measure
      # print(f'\tnumzeros {numzeros}')
      #create a zero-value 2D array of dim [classes,numzeros]
      midi_segment=np.zeros((self.num_classes,numzeros), dtype=np.int)      
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
          # print(f"qtimestamp {qtimestamp}. qduration {qduration}")
          # if timestamp+duration>=upto_miditick-from_miditick:
          #   duration=upto_miditick-from_miditick-timestamp

          
          # print(f'\tnote: {j}, timestamp {timestamp}, duration {duration}, label_id {label_id}, velocity {velocity}')
          # print(f'\tqtimestamp {qtimestamp}, qduration {qduration}\n')
          
          # form the final midisegment label 
          midi_segment[label_id][qtimestamp:qtimestamp+qduration]=velocity

          print(midi_segment.shape)
          print(midi_segment)
          break
      break
      midi_segments.append(midi_segment)

    return audio_segments,midi_segments