#Import libraries
import numpy as np
from torch.utils.data import Dataset
#from tqdm import tqdm_notebook as tqdm

import preprocess
#import augmentation


class Data(Dataset): 
  def __init__(self, csv,folders): #features,labels,folders,split,transforms):
    
    # create segments - timestamp , note-label , duration , velocity 
    data, labels = preprocess.create_segments(csv,folders)
    
    print(f'data[0] : {data[0]}')
    print(f'labels[0]: {labels[0]}') 
    # output --> labels[0]: [0.0036231916666666663, 'Crash_Cymbal', 0.10144936666666665, 60]
    # make this as follows: labels[0]: [ {model quarters according to quantization}, 'Crash_Cymbal'-->Class_id-integer, 0.10144936666666665-->duration in what units?, 60]

    
    """
    print('Loading_features......')

    #features, labels, folders
    self.indexes = [i for i, val in enumerate(folders) if val in split]
    self.data = [features[x] for x in self.indexes]
    self.labels = [labels[x] for x in self.indexes]

    #normalize
    self.data = np.asarray(self.data, dtype=np.float32)/255.0
 
    #image transforms
    self.data = augmentation.vision_augmentations(self.data,transforms)
    """
  def __len__(self):
    return len(self.data)
  def __getitem__(self, idx):#load data on demand
    return self.data[idx], self.labels[idx]