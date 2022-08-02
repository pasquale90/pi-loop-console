import mido 

#make a Note class
class Note:

  def __init__(self,channel,pitch,velocity,timestamp,timestamp_sec,duration,duration_sec):
    
    self.channel=channel
    self.classes=self._init_classes()
    self.pitch=self.remap_pitch(pitch)
    self.label=self.set_class(self.pitch)
    self.velocity=velocity
    self.timestamp=timestamp
    self.timestamp_sec=timestamp_sec
    self.duration=duration
    self.duration_sec=duration_sec
    
    self.relative_time=0 #in which part of the quarted is the note located

  def _init_classes(self):
    classes={"Bass":36,
        "Snare":38,
        "High_Tom":50,
        "Low_Mid_Tom":47,
        "High_Floor_Tom":43,
        "Open_Hi_Hat":46,
        "Closed_Hi_Hat":42,
        "Crash_Cymbal":49,
        "Ride_Cymbal":51,
        "other":0}
    return classes
  def get_label(self):
    return self.label
  def get_pitch(self):
    return self.pitch
  def get_numclasses(self):
    return len(self.classes)
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
    else:
      return 0
      
  def set_class(self,id):
    return [k for k,v in self.classes.items() if v == id][0]
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
