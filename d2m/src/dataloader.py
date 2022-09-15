from torch.utils.data import Dataset

class EGMD_dataloader(Dataset): 

  def convert_to_tensor(data):
      tensors = []
      for file in data:
        tensor = torch.Tensor(list(file))
        tensors.append(tensor)
      return tensors

  
  def _windows(data, window_size, hop_length):
    start = 0
    while start < len(data):
      yield start, start + window_size
      start += hop_length
  
  def __init__(self, waveforms,labels,folders,split_idxs):
    
    

    print('Loading_data......')

    #features, labels, folders
    self.indexes = split_idxs

    self.data = [waveforms[x] for x in self.indexes]
    self.labels = [labels[x] for x in self.indexes]

    #convert numpy to tensor
    self.data = convert_to_tensor(self.data)

  def __len__(self):
    return len(self.data)
  def __getitem__(self, idx):#load data on demand
    return self.data[idx], self.labels[idx]