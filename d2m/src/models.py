import torch
from torch import nn
from torch.autograd import Variable

class MANY2MANY_LSTM(nn.Module):
    def __init__(self, num_classes, hidden_size, seq_length):
        super(MANY2MANY_LSTM, self).__init__()
        self.hidden_size = hidden_size
        self.seq_length=seq_length

        # lstm1, lstm2, linear are all layers in the network
        self.lstm1 = nn.LSTMCell(self.hidden_size, self.hidden_size)
        self.lstm2 = nn.LSTMCell(self.hidden_size,1)
        self.linear = nn.Linear(1, num_classes)
        
    def forward(self, x, batch_size,future_preds=0):
        
        outputs= []
        h_t = torch.zeros(batch_size, self.hidden_size, dtype=torch.float32)
        c_t = torch.zeros(batch_size, self.hidden_size, dtype=torch.float32)
        h_t2 = torch.zeros(batch_size, 1, dtype=torch.float32)
        c_t2 = torch.zeros(batch_size, 1, dtype=torch.float32)
        
        for time_step in range(0,self.seq_length,1):
            # print(f'time_step {time_step}')
            # print(f'DEBUG:: input {x.shape}, seq_length {self.seq_length} , self.hidden_size*time_step:self.seq_length*(time_step+1) {self.hidden_size*time_step}:{self.hidden_size*(time_step+1)}')
            
            window=x[self.hidden_size*time_step:self.hidden_size*(time_step+1)]
            # print(f' input window {window.shape}')
            window=window.unsqueeze(0)
            # print(f' input window.unsqueeze(0) {window.shape}')

            h_t, c_t = self.lstm1(window, (h_t, c_t)) # initial hidden and cell states
            # print(f'lstm1:: h_t.shape {h_t.shape}, c_t.shape {c_t.shape}')
            
            h_t2, c_t2 = self.lstm2(h_t, (h_t2, c_t2)) # new hidden and cell states
            # print(f'lstm2:: h_t2.shape {h_t2.shape}, c_t2.shape {c_t2.shape}')
            
            output = self.linear(h_t2) # output from the last FC layer
            # print(f'output.shape {output.shape}')
            outputs.append(output)
        
        # print(f'outputs before cat :: len(outputs) {len(outputs)}, len(outputs[0]) {len(outputs[0])}')
        outputs = torch.cat(outputs, dim=1)
        # print(f'outputs = torch.cat(outputs, dim=1) {outputs.shape}')
        return outputs


def main4MANY2MANYLSTM():

    from audio import Audio
    import numpy as np
    from midifile import Midifile
    import pandas as pd
    import os
    import sys
    
# READ A SAMPLE
    sr=2048
    sample=Audio(audiopath="/data/pl/pi-loop-console/d2m/datasets/e-gmd_fixed_2000/eval/audio_inputs/1_funk-groove1_138_beat_4-4_1_1.wav",sampling_rate=sr,mono=True)
    data=sample.get_raw()
    print(len(data))
    # convert to tensors 
    data = Variable(torch.Tensor(data))
# MODEL
    #define device
    if torch.cuda.is_available():
        device=torch.device('cuda:0')
    else:
        device=torch.device('cpu')
    print(device)

    #introduce reproducibility
    seed = 7
    torch.manual_seed(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False    

    num_classes=10
    input_size=window_size=70
    hidden_size=quantization=32
    seq_length=window_size=int(2048/quantization) #=64 # VAR sr/quantization=2048/32(quantization)=64 samples window_size
    num_layers=1 # EXP num_layers

    model=MANY2MANY_LSTM( num_classes, hidden_size, seq_length)
    print("####################################################################")
    print(f"hidden_size {hidden_size} \nseq_length=quantization={seq_length} \ninput_size {input_size}")
    print("####################################################################")

    total_params = sum(p.numel() for p in model.parameters())
    print(f'model initialized with total : {total_params} parameters.')

# PASS THROUGH THE MODEL    
    output = model(data,batch_size=1)

if __name__=="__main__":

    main4MANY2MANYLSTM()
