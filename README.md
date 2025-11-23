# Flarksiesis v2.0

**Advanced LFO-Modulated Filter Effect**  
*by flarkAUDIO*

![Version](https://img.shields.io/badge/version-2.0.0-orange)
![License](https://img.shields.io/badge/license-GPL--3.0-orange)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-orange)

---

## Overview

Flarksiesis is a professional-grade audio plugin that combines powerful LFO modulation with versatile filtering capabilities. Featuring five distinct LFO waveforms, eight filter types, and comprehensive stereo processing, Flarksiesis delivers everything from subtle movement to extreme modulation effects.

### Key Features

- **5 LFO Waveform Shapes**: Sine, Triangle, Square, Saw, and Random
- **8 Filter Types**: Multiple lowpass, highpass, bandpass, notch, and allpass configurations
- **Tempo Synchronization**: Lock LFO rate to your DAW's tempo
- **Stereo Width Control**: Adjust the stereo field from mono to super-wide
- **Feedback Circuit**: Add resonance and character to the effect
- **Real-time LFO Visualization**: See your modulation in action
- **Resizable Interface**: Scale the UI from 600x450 to 1200x900
- **Cross-Platform**: VST3, Standalone, and LV2 (Linux) formats

---

## Installation

### Pre-built Installers (Recommended)

Download the installer for your platform from the [Releases](https://github.com/yourusername/flarksiesis/releases) page.

#### macOS (.pkg Installer)
1. Download `Flarksiesis-v2.0.0-macOS.pkg`
2. Double-click to run the installer
3. Choose which components to install (VST3 and/or Standalone)
4. Follow the installation wizard
5. The installer will place files in:
   - VST3: `/Library/Audio/Plug-Ins/VST3/`
   - Standalone: `/Applications/flarkAUDIO/`

#### Windows (.exe Installer)
1. Download `Flarksiesis-v2.0.0-Windows-Installer.exe`
2. Run the installer as Administrator
3. Choose installation components
4. The installer will place files in:
   - VST3: `C:\Program Files\Common Files\VST3\`
   - Standalone: `C:\Program Files\flarkAUDIO\Flarksiesis\`

#### Linux (.deb / .rpm / .tar.gz)
**Debian/Ubuntu:**
```bash
sudo dpkg -i Flarksiesis-v2.0.0-Linux.deb
```

**Fedora/Red Hat:**
```bash
sudo rpm -i Flarksiesis-v2.0.0-Linux.rpm
```

**Manual Installation:**
```bash
tar -xzf Flarksiesis-v2.0.0-Linux.tar.gz
sudo cp -r Flarksiesis.vst3 /usr/lib/vst3/
sudo cp -r Flarksiesis.lv2 /usr/lib/lv2/
sudo cp Flarksiesis /usr/bin/
```

### Manual Installation (From Binaries)

Download the latest release from the [Releases](https://github.com/yourusername/flarksiesis/releases) page.

#### Windows
1. Extract `Flarksiesis.vst3` 
2. Copy to: `C:\Program Files\Common Files\VST3\`

#### Linux
1. Extract `Flarksiesis.vst3` and/or `Flarksiesis.lv2`
2. VST3: Copy to `~/.vst3/`
3. LV2: Copy to `~/.lv2/`

#### macOS
1. Extract `Flarksiesis.vst3`
2. Copy to `/Library/Audio/Plug-Ins/VST3/` or `~/Library/Audio/Plug-Ins/VST3/`

---

## Building from Source

### Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler
- Git

### Build Instructions

```bash
# Clone the repository
git clone --recursive https://github.com/yourusername/flarksiesis.git
cd flarksiesis

# If you forgot --recursive, get JUCE:
git submodule update --init --recursive

# Create build directory
mkdir build
cd build

# Configure and build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Built plugins will be in build/Flarksiesis_artefacts/
```

### Linux-Specific Build (Arch)

```bash
# Install dependencies
sudo pacman -S base-devel cmake git

# For GUI support
sudo pacman -S webkit2gtk freetype2 libx11 libxext libxrandr libxinerama libxcursor

# Build as above
```

### Windows (Visual Studio)

```bash
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### macOS

```bash
cmake .. -G Xcode
cmake --build . --config Release
```

---

## Building Installers

### Quick Installer Build (macOS/Linux)

```bash
chmod +x scripts/build_installer.sh
./scripts/build_installer.sh
```

This will build the plugin and create a platform-specific installer in the `build/` directory.

### Manual Installer Build

#### macOS (.pkg)
```bash
# After building the plugin
cd build
chmod +x ../scripts/build_installer_macos.sh
../scripts/build_installer_macos.sh
```

Creates: `Flarksiesis-v2.0.0-macOS.pkg`

#### Windows (InnoSetup)
1. Install [Inno Setup 6](https://jrsoftware.org/isdl.php)
2. Build the plugin with CMake
3. Open `installer/windows/flarksiesis.iss` in Inno Setup
4. Click "Compile" to create the installer

Creates: `Flarksiesis-v2.0.0-Windows-Installer.exe`

#### Linux (DEB/RPM)
```bash
cd build
cpack -G "DEB;RPM;TGZ"
```

Creates package files in `build/` directory.

---

## User Guide

### LFO Section

**Rate**: Controls the speed of LFO modulation (0.01 - 20 Hz)
- When Tempo Sync is off: Direct frequency control
- When Tempo Sync is on: Musical divisions relative to DAW tempo

**Depth**: Amount of modulation applied to the filter frequency (0 - 100%)
- Higher values create more dramatic pitch sweeps
- Lower values provide subtle movement

**Waveform**: Shape of the LFO
- **Sine**: Smooth, classic modulation
- **Triangle**: Linear rise and fall
- **Square**: Abrupt switching between two states
- **Saw**: Gradual rise with instant reset
- **Random**: Smooth interpolated noise

**Tempo Sync**: Lock LFO rate to host tempo
- Automatically adjusts to tempo changes
- Musical divisions from 8 bars down to 1/32 notes

### Filter Section

**Frequency**: Center/cutoff frequency of the filter (20 - 20,000 Hz)
- This is the base frequency before LFO modulation
- Logarithmic scaling for musical response

**Resonance**: Filter emphasis at cutoff frequency (0.1 - 10.0)
- Higher values create more pronounced filtering
- Can add character and "ringing" to the sound

**Filter Type**: 
- **Lowpass 24dB**: Classic smooth lowpass (4-pole)
- **Highpass 24dB**: Removes low frequencies (4-pole)
- **Bandpass**: Allows only a band of frequencies through
- **Notch**: Removes a specific frequency band
- **Allpass**: Phase manipulation without amplitude change
- **LP 12dB**: Gentler lowpass slope (2-pole)
- **HP 12dB**: Gentler highpass slope (2-pole)
- **BP 12dB**: Gentler bandpass slope (2-pole)

### Output Section

**Mix**: Dry/Wet balance (0 - 100%)
- 0%: Pure dry signal
- 50%: Equal mix
- 100%: Pure wet signal

**Stereo Width**: Stereo field width (0 - 200%)
- 0%: Mono (identical L/R)
- 100%: Normal stereo
- 200%: Enhanced stereo width

**Feedback**: Signal fed back into the effect (0 - 95%)
- Adds resonance and character
- Higher values create more intense effects
- Use carefully to avoid instability

---

## Technical Specifications

- **Sample Rate Support**: 44.1 kHz - 192 kHz
- **Bit Depth**: 32-bit floating point internal processing
- **Latency**: Zero latency (no lookahead)
- **CPU Usage**: Optimized for real-time performance
- **Channels**: Stereo (mono compatible)

### DSP Details

- State Variable Filters with resonance control
- Smooth parameter interpolation to avoid clicks
- Optimized coefficient calculation
- Per-channel filter state for true stereo processing

---

## Preset Guide

### Subtle Movement
- Rate: 0.2 Hz
- Depth: 20%
- Waveform: Sine
- Filter: Lowpass
- Mix: 30%

### Classic Phaser
- Rate: 0.5 Hz
- Depth: 50%
- Waveform: Triangle
- Filter: Allpass
- Feedback: 50%

### Rhythmic Chop
- Rate: 4 Hz (or tempo-synced 1/4)
- Depth: 80%
- Waveform: Square
- Filter: Lowpass
- Mix: 70%

### Experimental Textures
- Rate: 8 Hz
- Depth: 90%
- Waveform: Random
- Filter: Bandpass
- Feedback: 70%
- Stereo Width: 150%

---

## Building with GitHub Actions

This repository includes automated CI/CD workflows that build the plugin for all platforms:

- **Windows**: VST3 + Standalone
- **Linux**: VST3, LV2, + Standalone
- **macOS**: VST3 + Standalone (Universal Binary)

Builds are triggered on push to `main` or when creating release tags.

---

## Development

### Project Structure

```
flarksiesis/
├── Source/
│   ├── PluginProcessor.h/cpp    # DSP engine
│   └── PluginEditor.h/cpp       # UI implementation
├── .github/workflows/           # CI/CD configurations
├── CMakeLists.txt              # Build configuration
├── README.md
└── LICENSE
```

### Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes with clear commit messages
4. Submit a pull request

---

## License

Flarksiesis is released under the GNU General Public License v3.0. See [LICENSE](LICENSE) for details.

This means you are free to:
- Use the plugin commercially
- Modify and distribute modifications
- Use in closed-source projects

As long as:
- You provide source code of modifications
- You keep the same license
- You credit the original author

---

## Credits

**Developer**: Flark  
**Brand**: flarkAUDIO  
**Framework**: JUCE 7.x  
**Inspired By**: Classic analog filter effects and modern modulation techniques

---

## Support

- **Issues**: Report bugs via [GitHub Issues](https://github.com/yourusername/flarksiesis/issues)
- **Discussions**: Feature requests and general discussion in [GitHub Discussions](https://github.com/yourusername/flarksiesis/discussions)

---

## Changelog

### v2.0.0 (Current)
- Complete rewrite in modern C++17
- Added 5 LFO waveform shapes
- Added 8 filter types
- Tempo synchronization
- Stereo width control
- Feedback circuit
- Real-time LFO visualization
- Resizable interface
- Cross-platform builds
- Optimized DSP performance

### v1.0.0 (Original)
- Initial release based on Charsiesis
- Basic LFO modulation
- Single filter type

---

**Made with ❤️ by flarkAUDIO**
