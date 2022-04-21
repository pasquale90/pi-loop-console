#Import libraries
import numpy as np
from torch.utils.data import Dataset
#from tqdm import tqdm_notebook as tqdm

import segment
import augmentation


class Data(Dataset): 
  def __init__(self, features,labels,folders,split,transforms):

    print('Loading_features......')

    #features, labels, folders
    self.indexes = [i for i, val in enumerate(folders) if val in split]
    self.data = [features[x] for x in self.indexes]
    self.labels = [labels[x] for x in self.indexes]

    #normalize
    self.data = np.asarray(self.data, dtype=np.float32)/255.0
 
    #image transforms
    self.data = augmentation.vision_augmentations(self.data,transforms)
    
  def __len__(self):
    return len(self.data)
  def __getitem__(self, idx):#load data on demand
    return self.data[idx], self.labels[idx]