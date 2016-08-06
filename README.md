# f0plugins
plugins for supercollider

plugins for [supercollider](http://supercollider.github.io) 3.6 and above.

* Astrocade - sound chip emulation
* Atari2600 - tia sound chip emulation
* AY8910 - sound chip emulation
* Beep - sound chip emulation
* MZPokey - another pokey sound chip emulation
* Nes2 - apu sound chip emulation
* Pokey - pokey sound chip emulation
* RedDPCM - two plugins for encoding and decoding differential pulse-code modulation signals
* RedLbyl - look before you leap
* RedNoise - a really bad pseudo-random noise generator
* RedPhasor and RedPhasor2 - two phasors that can loop
* SID6581f - sound chip emulation
* Slub - for generating cpu spikes
* SN76489 - sound chip emulation

see also <http://www.fredrikolofsson.com/pages/code-sc.html#plugins>

binaries
--

to download pre-made and ready-to-use binaries see <https://github.com/redFrik/f0plugins/releases>

compile
--

to build these yourself first install cmake and download the supercollider main source code via git, then open terminal and cd to the folder containing this README and do...

```shell
> mkdir build && build
> cmake -DSC_PATH=~/supercollider -DINSTALL_DESTINATION="/absolute/path" ..
> make install
```

edit `-DSC_PATH` above to point to your supercollider source directory, and edit `-DINSTALL_DESTINATION` to where you want the resulting plugins installed. on *osx* this will typically be `/Users/???/Library/Application Support/SuperCollider/Extensions` and on *linux* you can completely remove the -DINSTALL_DESTINATION use the default location.

NOTE: Not tested under Windows.
