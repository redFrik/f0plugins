# f0plugins

Unit-Generator Plugins (UGens) for [SuperCollider](https://supercollider.github.io)

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

See also <https://fredrikolofsson.com/code/sc/#plugins>

## Binaries

Download these plugins as ready-to-use binaries for macOS, Windows or Linux from here:

<https://github.com/redFrik/f0plugins/releases>

---

## Building

To compile the binaries yourself follow these instructions...

### Requirements

- CMake >= 3.5
- SuperCollider source code

### Building

Clone the project:

    git clone https://github.com/redFrik/f0plugins
    cd f0plugins
    mkdir build
    cd build

Then, use CMake to configure and build it:

    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . --config Release
    cmake --build . --config Release --target install

You may want to manually specify the install location in the first step to point it at your
SuperCollider extensions directory: add the option `-DCMAKE_INSTALL_PREFIX=/path/to/extensions`.

It's expected that the SuperCollider repo is cloned at `../supercollider` relative to this repo. If
it's not: add the option `-DSC_PATH=/path/to/sc/source`.
