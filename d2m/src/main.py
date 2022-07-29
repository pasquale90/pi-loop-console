import os
import math 
import paths
import preprocess
import data #preprocess
import hyperparams

if __name__=="__main__":
  
  #define paths - scan folders
  csv_path,folders=paths.get_data_subpaths(paths.datapath)  
  
  # create segments - timestamp , note-label , duration , velocity 
  audiolist, labels, velocities = preprocess.create_segments(csv_path,numMeasures=hyperparams.numMeasures)
  
  
  print(len(audiolist), len(labels), len(velocities))
  print(audiolist[0], labels[0], velocities[0])
  
  """ 
  # split data
  # ...
  train_data, train_labels = audiolist,labels
  valid_data = [],[]
  
  data.Data(train_data,labels)
  #valid_data = data.Data(csv,folders)
  """
  
  