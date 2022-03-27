import os
import mido #import MidiFile, tempo2bpm, tick2second
import pandas as pd
import librosa

from paths import *
from data.note import Note
from data.midifile import Midifile
from data.audio import Audio


#scan folders
csv_path,folders=get_data_subpaths(datapath)
csv=get_csv(csv_path)

audiofiles, midifiles=pathscan(len(folders))
print(len(audiofiles), len(midifiles))