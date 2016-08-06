# f0plugins
unit-generator plugins for [supercollider](http://supercollider.github.io) 3.6 and above.

* _Astrocade_ - sound chip emulation
* _Atari2600_ - tia sound chip emulation
* _AY8910_ - sound chip emulation
* _Beep_ - sound chip emulation
* _MZPokey_ - another pokey sound chip emulation
* _Nes2_ - apu sound chip emulation
* _Pokey_ - pokey sound chip emulation
* _RedDPCM_ - two plugins for encoding and decoding differential pulse-code modulation signals
* _RedLbyl_ - look before you leap
* _RedNoise_ - a really bad pseudo-random noise generator
* _RedPhasor_ and RedPhasor2 - two phasors that can loop
* _SID6581f_ - sound chip emulation
* _Slub_ - for generating cpu spikes
* _SN76489_ - sound chip emulation

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

edit `-DSC_PATH` above to point to your supercollider source directory (cloned from [here](https://github.com/supercollider/supercollider)), and edit `-DINSTALL_DESTINATION` to where you want the resulting plugins installed. on _osx_ this will typically be `/Users/???/Library/Application Support/SuperCollider/Extensions` and on _linux_ you can completely remove the -DINSTALL_DESTINATION use the default location.

NOTE: Not tested under Windows.
