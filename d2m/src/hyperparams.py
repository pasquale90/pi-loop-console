# preprocess.py
## audio params
audio_sr=2000 # will be implemented differently - sr will be adjustable according to the measure's duration


## midi params
numMeasures=1
quantization=32 # quarter=4 , eight-note=8 sixteen-note=16 etc
triplets=True


window_size= 4# Quantization Units in ms --> Given ms get num samples to form the window
hop_length=0

#UTILS.PY
# split : range from [0 to 1]
train_percent=0.7 



 
input [ 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0] # (3478,)
outp  [ 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0]
