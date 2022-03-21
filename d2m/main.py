import numpy as np
import os
import data


audiopaths,midipaths=data.pathscan()
print(f'There are {len(audiopaths)} audiofiles,\nThere are {len(midipaths)} midifiles.')

csv=data.get_csv()
