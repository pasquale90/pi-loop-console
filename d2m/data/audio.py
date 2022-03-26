import librosa



class Audio:
    def __init__(self,audiopath,sampling_rate,mono=True):

        self.mono=mono
        self.raw, self.sampling_rate =librosa.load(audiopath, sr=sampling_rate, mono=self.mono)


    def get_sampling_rate(self):
        return self.sampling_rate
    def is_mono(self):
        return self.mono
    def get_sample(self):
        return self.raw
    def get_duration_sec(self):
        return len(self.raw)/self.sampling_rate

    def audio2frames(self):
        frames = []
        #get windows out of the raw waveform
        for count_frames,(start,end) in enumerate(_windows(raw,window_size,hop_length)):
            if(len(raw[start:end]) == window_size):
                print(start,end)
                frame = raw[start:end]#rectangular window
    
'''
#define analysis parameters
def analysis_parameters():
    sampling_rate=16000  #for instance: if mode 16, sr = 16kHz
    window_size = sampling_rate
    overlap = int(window_size*0.75)
    hop_length = (window_size-overlap)
    return sampling_rate, window_size, hop_length


def _windows(data, window_size, hop_length):
    start = 0
    while start < len(data):
        yield start, start + window_size
        start += hop_length
                    
#sampling_rate=16000
raw, sr =librosa.load(audiosample, sr=sampling_rate, mono=True)
print(raw.shape)

frames = []
#get windows out of the raw waveform
for count_frames,(start,end) in enumerate(_windows(raw,window_size,hop_length)):
if(len(raw[start:end]) == window_size):
    print(start,end)
    frame = raw[start:end]#rectangular window
            



def preprocess_data(audio_path, folds, files, mode):
    
  def _windows(data, window_size, hop_length):
    start = 0
    while start < len(data):
      yield start, start + window_size
      start += hop_length

  def normalize(data):#in the space [-1,1]
    #return (data - np.min(data)) / (np.max(data) - np.min(data))#[0,1]
    return (2*(data-np.min(data))/(np.max(data) - np.min(data)))-1
  

  features = []
 
  extr = True
  if extr == True:
    
    print('Preprocessing data ........ ')
    
    sampling_rate, window_size, hop_length = analysis_parameters(mode)
    print(f'sampling_rate: {sampling_rate}, window_size: {window_size}, hop_length: {hop_length}')

    shape_print=True


    for count_files, file in tqdm(enumerate(audiofiles)):
      name = file.split('.wav')[0]

      #readfile
      raw, sr =librosa.load(audio_path+file, sr=sampling_rate, mono=True)
      #print(f'{file} had length {len(raw)}')

      #normalize
      raw = normalize(raw)
      
      frames = []
      #get windows out of the raw waveform
      for count_frames,(start,end) in enumerate(_windows(raw,window_size,hop_length)):
        if(len(raw[start:end]) == window_size):
          #print(start,end)
          frame = raw[start:end]#rectangular window

          #flatten
          #implemented inside the model

          frames.append(frame)
      
      features.append(frames)
     

      if shape_print:
            print('\nFeature Shape Check\n')
            print(f'raw has len:{len(raw)/sampling_rate}')
            print(f'Postprocessed feature has shape : {np.asarray(features).shape} with min:{np.asarray(features[0]).min()} and max:{np.asarray(features[0]).max()}]')
            print()
            shape_print = False

  #"""
  print('len(features)-features',len(features))
  print('len(features[0])-freq_domain',len(features[0]))
  print('labels',len(labels))
  #"""
  return features

#get analysis parameters
sampling_rate, window_size, hop_length = analysis_parameters(mode)
print(f'sampling_rate: {sampling_rate}, window_size: {window_size}, hop_length: {hop_length}')

#extract_features       
features = preprocess_data(audio_path, folds, audiofiles, mode)
#features, labels, folders = extract_mel_spectogram(audio_path,us8k,folds,audiofiles,sampling_rate,hop_length,fft_points,mel_bands)
print(f' feature\'s len : {len(features)}, labels : {len(labels)}, folders : {len(folders)}')
'''