import librosa

class Audio:
    def __init__(self,audiopath,sampling_rate,mono=True):

        self.mono=mono
        self.raw, self.sampling_rate =librosa.load(audiopath, sr=sampling_rate, mono=self.mono)


    def get_sampling_rate(self):
        return self.sampling_rate
    def is_mono(self):
        return self.mono
    def get_raw(self):
        return self.raw
    def get_duration_sec(self):
        return len(self.raw)/self.sampling_rate

