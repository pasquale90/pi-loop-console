import os
import math 
import paths
import preprocess
import data #preprocess
import hyperparams
import split
import dataloader 

if __name__=="__main__":
  
  #define paths - scan folders
  csv_path,folders=paths.get_data_subpaths(paths.datapath)  

  # create segments - timestamp , note-label , duration , velocity 
  audiolist, labelist = preprocess.create_segments(csv_path,audio_sr=hyperparams.audio_sr,numMeasures=hyperparams.numMeasures,quantization=hyperparams.quantization)
  
  
  print(len(audiolist), len(labelist))
  
  print(audiolist[0].shape)
  for row in labelist[0]:
    print(row)

  # split data
  num_segments=len(audiolist)[0]
  train_split, valid_split = utils.split_egmd(hyperparams.train_percent, num_segments)
  print(f'Train split: {len(train_split)} samples, Valid split: {len(valid_split)} samples. Total: {num_segments} samples.')

  # data loader 
  data,labels=dataloader.EGMD_dataloader

  

  # model

  # code for train - validate
  
  """ 
  # split data
  # ...
  train_data, train_labels = audiolist,labels
  valid_data = [],[]
  
  data.Data(train_data,labels)
  #valid_data = data.Data(csv,folders)
  """
  
  