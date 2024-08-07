# Future work section
<h4>Here are all the items that have been reported and addressed for the future. This listing is summarized as follows:</h4>

- [Bugs](#bugs)
- [Improvements](#improvements)
- [Additions](#additions)
- [Upgrades](#upgrades)
- [Ambition](#ambition)
- [Milestones](#milestones-achieved)

## Bugs
Here is the listing of bugs found and require fixing:

- [ ] channel: fix dubbing
    - if dub_length < loop_length, samples not aligned are discarded from the data[rec_state]
- [ ] channel: fix volume initialization
- [ ] audio_server/monitor_client: return info on the sample rate value configured after initialization.
    - rebuild the application or configure it properly on initialization with the acceptable settings.
- [x] metronome: 
    - [x] fix alter_tempo()
    - [x] move tick_tock to the interface thread (make use of a conditional variable)
- [ ] looper: handle exceptional case:
    - erasing the first channel, while another channel is recording ..
    - loop length becomes 0, while the other channel is still recording
    - new loop length defined. The behavior will probably be undefined.
- [ ] mixer: add reset?

## Improvements
Here is the listing of existing features that require curation.

- [x] configuring Piloop. Define as configuration parameter the following variables:
    - [x] cmake/make: MODE and DEVICE
    - [x] keyboards: move eventDevice 
    - [x] audio_settings: SAMPLE_RATE and BUFFER_SIZE
    - [x] gpio/buttons : I2C_BUS
        - [x] automatic detection of the i2c-bus used.
- [ ] channel: stereo output not supported --> fix in looper._fuse_input()
- [ ] mixer: conform to both mono and stereo out
- [ ] mixer: move dataPath to config
- [x] metronome: Instead of timing, use loop length to calculate the #buffers per quarter unit
- [ ] handshake: add latency status in _info_control()
- [ ] monitor: set idle when all monitor states (ARM/MNTR) are disabled.
- [ ] audio_settings: replace macros with a namespace of constexpr variables

## Additions
Here is the list of missing features needed to make the Piloop 1.0 update:

- [ ] audio_settings.h: add support for phones and sub_out 
- [ ] effects: add effects
- [ ] session: save_session feature
    - [ ] config: add support for saving/loading metadata
    - [ ] looper: save/load looper state
        - save/load channels
        - save/load metronome
- [ ] session/config: save metadata for when saving jams. This will serve as a method to import files in DAWs that can be used as a songwriting guide afterward.

## Upgrades

- [ ] add a drum transcription tool
- [ ] integrate Piloop in a box case.
- [ ] transfer application into a new microcontroller (i.e. stm32 or nvidia-jetson module)

## Ambition
Make Piloop a fully-equipped loop station console.

## Milestones achieved
- [x] multi-threading
- [x] start jack audio server internally
- [x] Raspberry Pi deployment
- [x] optimized audio operations