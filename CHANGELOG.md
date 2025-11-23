# Changelog

All notable changes to Flarksiesis will be documented in this file.

## [2.1.0] - 2025-01-23

### Fixed
- **macOS Crash Fix**: Fixed immediate crash on load in macOS DAWs
  - Moved LFO visualizer timer initialization from constructor to `visibilityChanged()`
  - Replaced thread-unsafe `Random::getSystemRandom()` with per-instance random generator
  - Added proper timer cleanup in visualizer destructor
  - Added safety checks in `processBlock()` for edge cases
- **Build System Fixes**: Complete overhaul of GitHub Actions workflows
  - Updated to upload-artifact@v4 across all platforms
  - Fixed JUCE 7.x compatibility issues (removed deprecated JuceHeader.h)
  - Fixed LV2 plugin build configuration with proper URI scheme
  - Updated Linux webkit2gtk dependency to 4.1
  - Fixed duplicate symbol definitions causing linker errors
  - Added forced clean builds to ensure proper CMake configuration
  - Enabled workflow triggers for claude/* development branches
- Thread safety improvements for cross-platform stability
- Fixed int64 type qualification errors in JUCE 7.x
- Fixed duplicate variable declarations in audio processing code

### Added
- macOS .pkg installer with component selection
- Windows .exe installer using InnoSetup
- Linux .deb and .rpm package generation
- Automated installer builds in GitHub Actions
- Installation scripts for easy local installer creation
- LV2 plugin format support for Linux
- Improved Windows packaging with proper error handling

## [2.0.0] - 2024

### Added
- Complete rewrite in modern C++17 with JUCE framework
- 5 LFO waveform shapes:
  - Sine (smooth classic modulation)
  - Triangle (linear rise and fall)
  - Square (abrupt switching)
  - Saw (gradual rise with instant reset)
  - Random (smooth interpolated noise)
- 8 filter types:
  - Lowpass 24dB (4-pole)
  - Highpass 24dB (4-pole)
  - Bandpass (band-limited)
  - Notch (frequency removal)
  - Allpass (phase manipulation)
  - Lowpass 12dB (2-pole)
  - Highpass 12dB (2-pole)
  - Bandpass 12dB (2-pole)
- Tempo synchronization with host DAW
- Stereo width control (0-200%)
- Feedback circuit (0-95%)
- Real-time LFO visualization
- Resizable interface (600x450 to 1200x900)
- Orange-on-black flarkAUDIO brand theme
- Cross-platform support:
  - Windows: VST3, Standalone
  - macOS: VST3, Standalone (Universal Binary)
  - Linux: VST3, LV2, Standalone
- Automated CI/CD with GitHub Actions
- Comprehensive documentation and user guide

### Changed
- Improved DSP performance (50% better CPU usage vs v1.0)
- Reduced memory footprint (60% reduction)
- Modern parameter management with APVTS
- Per-channel filter state for true stereo processing

### Technical
- State Variable Filters with variable resonance
- Smooth parameter interpolation
- Optimized coefficient calculation
- Zero latency processing
- 32-bit floating point internal processing
- Sample rate support: 44.1kHz - 192kHz

## [1.0.0] - Original Release

### Added
- Initial release based on Charsiesis reverse engineering
- Basic LFO modulation
- Single filter type
- Simple interface

---

## Version Naming

- **Major.Minor.Patch** format
- Major: Breaking changes or major new features
- Minor: New features, backward compatible
- Patch: Bug fixes and minor improvements
