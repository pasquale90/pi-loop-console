import os
from paths import get_fold_path,set_session_path,get_session_path
import numpy as np

def split_egmd(train_percent,numSamples):

  numTrain=int(train_percent*numSamples)
  numValid=int(numSamples-numTrain)

  total_idxs=[i for i in range(numSamples)]
  np.random.shuffle(total_idxs)

  train_split_idxs=total_idxs[:numTrain]
  valid_split_idxs=total_idxs[-numValid:]

  return train_split_idxs, valid_split_idxs



if __name__=="__main__":

  split_egmd(0.7,12)