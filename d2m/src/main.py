import os
import sys
import math 
import paths
import preprocess
import data #preprocess
import hyperparams
import dataloader 

if __name__=="__main__":
  
  #define paths - scan folders
  csv_path,folders=paths.get_data_subpaths(paths.datapath)  

  # create segments - timestamp , note-label , duration , velocity
  prepare_data = preprocess.Preprocess()
  csv_path='/'.join(csv_path.split('/')[:-1])+"/demo_csv.csv"
  audiolist, labelist = prepare_data.create_segments(csv_path,hyperparams.audio_sr,hyperparams.numMeasures,hyperparams.quantization)  






  print(len(audiolist), len(labelist))
  

  sys.exit(0)
  
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
  
  