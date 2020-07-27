# f0plugins
unit-generator plugins for [supercollider](https://supercollider.github.io)

* __Astrocade__ - sound chip emulation
* __Atari2600__ - tia sound chip emulation
* __AY8910__ - sound chip emulation (also __AY8910Square__)
* __Beep__ - sound chip emulation
* __MZPokey__ - another pokey sound chip emulation
* __Nes2__ - apu sound chip emulation (also __Nes2DMC__, __Nes2Noise__, __Nes2Square__ and __Nes2Triangle__)
* __Pokey__ - pokey sound chip emulation (also __PokeySquare__)
* __RedDPCMdecode__ and __RedDPCMencode__ - plugins for encoding and decoding differential pulse-code modulation signals
* __RedLbyl__ - look before you leap
* __RedNoise__ - a really bad pseudo-random noise generator
* __RedPhasor__ and __RedPhasor2__ - two phasors that can loop
* __SID6581f__ - sound chip emulation
* __Slub__ - for generating cpu spikes
* __SN76489__ - sound chip emulation

see also <https://fredrikolofsson.com/code/sc/#plugins>

binaries
--

to download pre-made and ready-to-use binaries see <https://github.com/redFrik/f0plugins/releases>

compile
--

to build these yourself first install cmake and download the supercollider main source code via git, then open terminal and cd to the folder containing this README and do...

```shell
> mkdir build && cd build
> cmake -DSC_PATH=~/supercollider -DINSTALL_DESTINATION="/absolute/path" ..
> make install
```

edit `-DSC_PATH` above to point to your supercollider source directory (cloned from [here](https://github.com/supercollider/supercollider)), and edit `-DINSTALL_DESTINATION` to where you want the resulting plugins installed. on __osx__ this will typically be `/Users/???/Library/Application Support/SuperCollider/Extensions` and on __linux__ you can completely remove the `-DINSTALL_DESTINATION` to use the default location.

NOTE: Not tested under Windows.
