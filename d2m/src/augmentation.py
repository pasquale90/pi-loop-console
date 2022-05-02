import numpy as np
import librosa

#AUDIO AUGMENTATION FUNCTIONS_ synthetic data
def audio_augmentation(data, sr, class_conditional, shift_time=True, thresshold=0.5):

  #add_white_noise to the signal
  def white_noise(data): 
    noiz = np.random.randn(len(data))
    mean_intensity = np.sum(np.square(data))/len(data)
    data_wn = data + noiz*0.75* mean_intensity
    return data_wn

  #pitch shifting of wave by a random factor value in the space [-1,1].  Permissible : -5 <= x <= 5
  def pitch_shift(data,sr):
    detune = np.random.uniform(low=-2.5,high=-1.75,size=None)
    overtune = np.random.uniform(low=1.75,high=2.5,size=None)
    shift_factor = np.random.choice([detune,overtune])
    pitch_shifted = librosa.effects.pitch_shift(data,sr,n_steps=shift_factor)
    return pitch_shifted

  def soft_pitch_shift(data,sr):
    detune = np.random.uniform(low=-1.0,high=-0.5,size=None)
    overtune = np.random.uniform(low=0.5,high=1.0,size=None)
    shift_factor = np.random.choice([detune,overtune])
    pitch_shifted = librosa.effects.pitch_shift(data,sr,n_steps=shift_factor)
    return pitch_shifted
