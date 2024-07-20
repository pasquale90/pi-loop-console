# Manual

#### Here you may find instructions on how to use Piloop in **PC mode**.
For the **RPI mode**, the instructions will be updated soon. Up till then, visit the [setup page](rpi-setup.md) to get an overview of the current installation setup.

## Contents 

- [Interaction controls](#interaction-controls)
- [Display interface](#display-interface)
- [Action list](#action-list)

## Interaction controls

The list of keyboard controls to operate Piloop. A detailed explanation of the available actions can be found in the [action list](#action-list) section.

Control | Action on prolonged pressing | Action on instant pressing
:--- | :---: | ---:
z |	undub last track of channel 1 | record/dub on channel 1
x | erase all tracks of channel 1 | stop playback/recording on channel 1 
c | undub last track of channel 2 | record/dub on channel 2
v | erase all tracks of channel 2 | stop playback/recording on channel 2
b | undub last track of channel 3 | record/dub on channel 3
n | erase all tracks of channel 3 | stop playback/recording on channel 3
a | erase all tracks from all channels | pause/unpause all tracks from all channels
s | save jam | - 
q | - | Enable/disable ARM for channel 1
w | - | Enable/disable MNTR for channel 1
r | - | Enable/disable ARM for channel 2
t | - | Enable/disable MNTR for channel 2
1 | - | Enable/disable effect 1 for channel 1
2 | - | Enable/disable effect 2 for channel 1
3 | - | Enable/disable effect 3 for channel 1
4 | - | Enable/disable effect 1 for channel 2
5 | - | Enable/disable effect 2 for channel 2
6 | - | Enable/disable effect 3 for channel 2
p | alters the tempo | tap tempo
j | - | go to the previous session
l | - | go to the next session
k | - | save current session (presets and loop-channels)
Fn^F1 | - |  Decrease volume of loop-channel 1
Fn^F2 | - |  Increase volume of loop-channel 1
Fn^F5 | - |  Decrease volume of loop-channel 2
Fn^F6 | - |  Increase volume of loop-channel 2
Fn^F9 | - |  Decrease volume of loop-channel 3
Fn^F10 | - | Increase volume of loop-channel 3
Esc | - | Exit Piloop 

**NOTE**: The following features are not supported yet:
- Enabling/disabling of effect states is supported, but no effects are applied to the input signal.
- When saving/changing a session, the looper-tracks are not saved/loaded, but the current presets only.
For more information, visit the [future work list](future_work.md).

## Display interface

In `PC mode`, users are able to inspect various states of the program related to the monitor state, the looper state, the metronome state etc.

<h1 align="left">
  <img src="../imgs/piloop-demo.gif" width="1000"></a>
</h1>  

## Action list

The included features of Piloop involve the following actions:

Action | Description
:--- | ---:
record | record a track from one or more input signals to one of the looper-channels
playback |	playback recorded samples from the corresponding looper-channel
dub | record on top of other records to a non empty looper-channel
pause |	pause playback on a looper-channel
erase |	erase all tracks of a looper-channel
start/stop all|	unpause/pause all tracks of all looper-channels
save jam | save the last minutes of your musical session. The max value is set to 5 minutes.
arm/disarm|	include/exclude the input signal from the correspondig input channel to the current looper record
toggle monitor state |	enable/disable the audio streaming of the input signal from the correspondig input channel
tap tempo |	set the tempo manually
alter tempo | alter the current tempo
save session |	save the current session (presets and loop-tracks only)
change session | change the current session (presets and loop-tracks only)
change volume |	change the volume state of a looper-channel
shutdown | turn off Piloop

---
