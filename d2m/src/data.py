#Import libraries
import numpy as np
import torch
from torch.utils.data import Dataset
from torch.autograd import Variable
#from tqdm import tqdm_notebook as tqdm


#import augmentation


class Data(Dataset): 
  def __init__(self, data,labels): #features,labels,folders,split,transforms):

    print('Loading_features......')    
    
    
    
    print(f'data.shape: {data.shape}') 
    print(f'labels.shape: {labels.shape}') 
    
    
    """
    #normalize
    self.data = np.asarray(self.data, dtype=np.float32)/255.0

    #image transforms
    self.data = augmentation.vision_augmentations(self.data,transforms)
    """
    
    
    
    
    # convert to tensors
    data = Variable(torch.Tensor(data))
    labels = Variable(torch.Tensor(labels))
    data =  torch.reshape(data,(data.shape[0], 1, data.shape[1]))
    print(f'data[0] : {data[0]}')
    print(f'labels[0]: {labels[0]}') 
    
    # output --> labels[0]: [0.0036231916666666663, 'Crash_Cymbal', 0.10144936666666665, 60]
    # make this as follows: labels[0]: [ {model quarters according to quantization}, 'Crash_Cymbal'-->Class_id-integer, 0.10144936666666665-->duration in what units?, 60]

    
    
  def __len__(self):
    return len(self.data)
  def __getitem__(self, idx):#load data on demand
    return self.data[idx], self.labels[idx]