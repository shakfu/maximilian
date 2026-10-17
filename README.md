```
  _____ _____ ___   __ _  _____  __ __   __ ____  ____
 /     \\_   \\  \/  /  |/     \|  |  | |  \_   \/    \
|  Y Y  \/ /_ \>    <|  |  Y Y  \  |  |_|  |/ /_ \  Y  \
|__|_|  (___  /__/\__\__|__|_|  /__|____/__(___  /__|  /
      \/    \/                \/               \/    \/
```
![version](https://img.shields.io/badge/version-2.4-red)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/mimic-sussex/eppEditor/blob/master/LICENSE)

<br />

### What's Maximilian?

Maximilian is a cross-platform and multi-target audio synthesis and signal processing library. It was written in C++ and provides bindings to Javascript. It's compatible with native implementations for MacOS, Windows, Linux and iOS systems, as well as client-side browser-based applications. Maximilian is self-contained, and compiles without dependencies. The main features are:

- sample playback, recording and looping
- support for WAV and OGG files.
- a selection of oscillators and filters
- enveloping
- multichannel mixing for 1, 2, 4 and 8 channel setups
- controller mapping functions
- effects including delay, distortion, chorus, flanging
- granular synthesis, including time and pitch stretching
- atom synthesis
- real-time music information retrieval functions: spectrum analysis, spectral features, octave analysis, Bark scale analysis, and MFCCs
- example projects for Windows and MacOS, susing command line and OpenFrameworks environments
- example projects for Firefox and Chromium-based browsers using the Web Audio API ScriptProcessorNode (deprecated!)
- example projects for Chromium-based browsers using the Web Audio API AudioWorklet (e.g. Chrome, Brave, Edge, Opera, Vivaldi)
- will run on embedded systems (e.g. ESP32, Pi Pico)

### Documentation

in [docs/index.html](docs/index.html)


### Basic Examples



Examples demonstrating different features can be found in the maximilian_examples folder. Build the library, examples and tests from the repository root:

```
cmake -S . -B build
cmake --build build
./build/cpp/commandline/example_1_TestTone
```

Each example folder `N.Name` becomes the target `example_N_Name`. Libraries are static by default; pass `-DBUILD_SHARED_LIBS=ON` for a shared `maximilian`. Other options:

- `MAXIMILIAN_VORBIS` (default `OFF`): OGG loading via stb_vorbis.
- `MAXIMILIAN_BUILD_COMMANDLINE` (default `ON` at top level): the RtAudio player, examples and tests.
- `MAXIMILIAN_INSTALL` (default `ON` at top level): install rules and an exported `maximilian::maximilian` target.
- `MAXIMILIAN_BUILD_TESTS` (default `ON` at top level): unit tests in `tests/`, plus offline smoke runs of every program in `cpp/commandline/tests/` and `cpp/commandline/maximilian_examples/`. Run them with `ctest --test-dir build`.
- `MAXIMILIAN_SANITIZE` (default `OFF`): build everything with AddressSanitizer and UBSan; any report fails the test.

To use the library from another CMake project, either `add_subdirectory(maximilian)` or install it and call `find_package(maximilian)`, then link `maximilian::maximilian`.

### Web Audio

A transpiled javascript version of the library is included in this repository, for both Script Processor Nodes and AudioWorklets. Try this out at (https://mimicproject.com/guides/maximJS).

To run this on your on site, locally, or on GitHub Pages, check out [this repo](https://github.com/Louismac/maximilian-js-local).


### Mac OS XCode Project

You can run the examples using the 'maximilianTest' XCode 3 project provided.


### MS Windows Visual Studio Project

This is in the maximilianTestWindowsVS2010 folder. You will need to install the DirectX SDK, so that the program can use DirectSound.


### Command Line Compilation in Mac OS

> g++ -Wall -D__MACOSX_CORE__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -framework CoreAudio -framework CoreFoundation -lpthread

For M1 Mac OS
> g++ -Wall -D__MACOSX_CORE__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -framework CoreAudio -framework CoreServices -framework AudioToolbox -framework AudioUnit -framework Accelerate -lpthread

> ./maximilian


### Command Line Compilation in Linux

With OSS:
> g++ -Wall -D__LINUX_OSS__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -lpthread

With ALSA:
> g++ -Wall -D__LINUX_ALSA__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp -lasound -lpthread

With Jack:
> g++ -Wall -D__UNIX_JACK__ -o maximilian main.cpp RtAudio.cpp player.cpp maximilian.cpp `pkg-config --cflags --libs jack` -lpthread

then:
> ./maximilian



### OpenFrameworks Project

Maximilian works well with the OpenFrameworks C++ creative coding toolkit (http://www.openframeworks.cc).

In the ofxMaxim directory you will find examples to run in Windows, OSX and iOS, including FFT analysis and granular synthesis.

You can install the ofxMaxim addon by copying the ofxMaxim/ofxMaxim folder into your openframeworks addons directory.

Important: when using Maximilian on OSX, link against the Accelerate framework.



### Developer Notes

[see developer_notes.md](developer_notes.md)





