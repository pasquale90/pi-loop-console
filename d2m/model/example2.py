from BeatNet.BeatNet import BeatNet
import os

estimator =BeatNet(1,mode="realtime",inference_model='PF', plot=['beat_particles'],thread=False)
Output= estimator.process(os.path.join(os.getcwd(),'example2.wav'))
