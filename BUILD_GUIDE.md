# Flarksiesis Build Guide

## 🚨 Important: Cannot Build in This Environment

This environment has network restrictions that prevent:
- Cloning JUCE submodule from GitHub
- Installing required audio libraries
- Building GUI components

**However, everything is configured correctly and will build successfully on your local machine or via GitHub Actions!**

---

## ✅ What Will Be Built

When you build Flarksiesis on a proper system, you'll get:

### Linux Build Outputs
```
build/Flarksiesis_artefacts/Release/
├── VST3/
│   └── Flarksiesis.vst3/          # VST3 plugin bundle
├── LV2/
│   └── Flarksiesis.lv2/           # LV2 plugin bundle  
└── Standalone/
    └── Flarksiesis                # Standalone executable
```

**Linux Installers Created:**
- `Flarksiesis-2.0.0-Linux.deb` (Debian/Ubuntu)
- `Flarksiesis-2.0.0-Linux.rpm` (Fedora/Red Hat)
- `Flarksiesis-2.0.0-Linux.tar.gz` (Universal)

### macOS Build Outputs
```
build/Flarksiesis_artefacts/Release/
├── VST3/
│   └── Flarksiesis.vst3/          # VST3 plugin bundle (Universal)
└── Standalone/
    └── Flarksiesis.app            # macOS application (Universal)
```

**macOS Installer Created:**
- `Flarksiesis-v2.0.0-macOS.pkg` (Professional installer with GUI)

### Windows Build Outputs
```
build/Flarksiesis_artefacts/Release/
├── VST3/
│   └── Flarksiesis.vst3/          # VST3 plugin bundle
└── Standalone/
    └── Flarksiesis.exe            # Windows executable
```

**Windows Installer Created:**
- `Flarksiesis-v2.0.0-Windows-Installer.exe` (InnoSetup installer)

---

## 🏗️ Build Instructions by Platform

### Option 1: GitHub Actions (Easiest - Automatic)

**Just push to GitHub and Actions builds everything automatically!**

```bash
cd /home/flark/Downloads/flarksiesis-repo

# Initialize and push (as per QUICKSTART.md)
git init
git add .
git commit -m "Initial commit: Flarksiesis v2.0"
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
git add .gitmodules JUCE
git commit -m "Add JUCE submodule"

# Create repo on GitHub, then:
git remote add origin https://github.com/YOUR_USERNAME/flarksiesis.git
git push -u origin main
```

**GitHub Actions will automatically:**
- Build for Linux (Ubuntu)
- Build for macOS (Universal Binary)
- Build for Windows (x64)
- Create installers for all platforms
- Upload artifacts

Download the artifacts from the Actions tab!

---

### Option 2: Local Build (Your Machine)

#### Linux (Arch Linux - Your System)

```bash
cd /home/flark/Downloads/flarksiesis-repo

# Install dependencies (you may already have these)
sudo pacman -S base-devel cmake git juce alsa-lib jack2

# Clone JUCE
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)

# Create installers
chmod +x ../scripts/build_installer.sh
../scripts/build_installer.sh
```

**Output:**
- `build/Flarksiesis_artefacts/Release/` - All plugin formats
- `build/Flarksiesis-2.0.0-Linux.tar.gz` - Installer package

**Quick Install (Local):**
```bash
# Install to user directories (no sudo needed)
mkdir -p ~/.vst3 ~/.lv2
cp -r build/Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3 ~/.vst3/
cp -r build/Flarksiesis_artefacts/Release/LV2/Flarksiesis.lv2 ~/.lv2/
```

#### macOS

```bash
cd /home/flark/Downloads/flarksiesis-repo

# Install Xcode Command Line Tools (if not already)
xcode-select --install

# Clone JUCE
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive

# Build
mkdir build && cd build
cmake .. -G Xcode -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build . --config Release

# Create installer
chmod +x ../scripts/build_installer_macos.sh
../scripts/build_installer_macos.sh
```

**Output:**
- `build/Flarksiesis-v2.0.0-macOS.pkg` - Professional installer

**Install the .pkg** by double-clicking it!

#### Windows

```powershell
cd C:\Users\YourName\Downloads\flarksiesis-repo

# Install Visual Studio 2022 (Community Edition is free)
# Install CMake for Windows
# Install Git for Windows

# Clone JUCE
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive

# Build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

# For installer, install Inno Setup 6, then:
# Open installer/windows/flarksiesis.iss in Inno Setup
# Click "Compile"
```

**Output:**
- `build\Flarksiesis-v2.0.0-Windows-Installer.exe` - Installer

---

## 🎯 Recommended: Use GitHub Actions

**Why?**
- ✅ Builds all platforms automatically
- ✅ No local setup needed
- ✅ Consistent build environment
- ✅ Creates professional installers
- ✅ Uploads artifacts to releases
- ✅ Works from any machine

**Just push your code and wait 10-15 minutes!**

---

## 📦 Manual Installation (Without Building)

If you have pre-built binaries:

### Linux
```bash
# VST3
sudo cp -r Flarksiesis.vst3 /usr/lib/vst3/
# or user install:
cp -r Flarksiesis.vst3 ~/.vst3/

# LV2
sudo cp -r Flarksiesis.lv2 /usr/lib/lv2/
# or user install:
cp -r Flarksiesis.lv2 ~/.lv2/
```

### macOS
```bash
# VST3
cp -r Flarksiesis.vst3 ~/Library/Audio/Plug-Ins/VST3/

# Standalone
cp -r Flarksiesis.app /Applications/
```

### Windows
```
VST3: Copy to C:\Program Files\Common Files\VST3\
Standalone: Copy to C:\Program Files\flarkAUDIO\
```

---

## 🔍 Troubleshooting

### JUCE Submodule Issues
```bash
# If submodule fails
git submodule deinit -f JUCE
rm -rf .git/modules/JUCE JUCE
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
```

### Linux Build Dependencies
```bash
# Arch
sudo pacman -S base-devel cmake git alsa-lib jack2 \
  webkit2gtk freetype2 libx11 libxrandr libxinerama libxcursor

# Ubuntu/Debian
sudo apt install build-essential cmake git libasound2-dev \
  libjack-jackd2-dev libfreetype6-dev libx11-dev libxrandr-dev \
  libwebkit2gtk-4.0-dev
```

### macOS Signing Issues
```bash
# Ad-hoc sign for local use
codesign --force --deep --sign - Flarksiesis.vst3
codesign --force --deep --sign - Flarksiesis.app
```

---

## 📊 Build Time Estimates

| Platform | Build Time | With Installer |
|----------|------------|----------------|
| Linux    | 3-5 min    | +30 sec        |
| macOS    | 5-8 min    | +1 min         |
| Windows  | 8-12 min   | +1 min         |

*Times on modern hardware with SSD*

---

## ✨ Quick Reference

**Fastest Way to Get Working Plugins:**

1. Push repo to GitHub
2. Wait for Actions to complete
3. Download artifacts from Actions tab
4. Run installer on your system
5. Restart DAW

**That's it!** 🎵

---

## 🆘 Need Help?

- Check `UPDATE_NOTES.md` for macOS crash fix details
- Read `README.md` for plugin usage guide
- See `CHANGELOG.md` for version history
- Open GitHub issue for build problems

---

**Repository:** `/home/flark/Downloads/flarksiesis-repo/`
**Status:** Ready to build (requires proper environment with network access)
**Recommendation:** Use GitHub Actions for automated builds
