#!/bin/bash
# Flarksiesis GitHub Push Script
# For user: flarkflarkflark

set -e

echo "=================================="
echo "Flarksiesis GitHub Push Script"
echo "User: flarkflarkflark"
echo "=================================="
echo ""

# Navigate to repository
cd /home/flark/Downloads/flarksiesis-repo

# Initialize git
echo "Initializing git repository..."
git init

# Configure git (change email if needed)
git config user.name "flarkAUDIO"
git config user.email "flarkaudio@example.com"  # Change this to your real email!

# Add and commit all files
echo "Committing files..."
git add .
git commit -m "Initial commit: Flarksiesis v2.0.1

- Fixed macOS crash (timer and thread-safety issues)
- Added professional installers (.pkg, .exe, .deb, .rpm)
- Complete C++17 rewrite with JUCE
- 5 LFO waveforms, 8 filter types
- Tempo sync, stereo width, feedback controls
- Cross-platform builds with GitHub Actions"

# Add JUCE submodule
echo "Adding JUCE submodule..."
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
git add .gitmodules JUCE
git commit -m "Add JUCE framework as submodule"

echo ""
echo "=================================="
echo "NEXT STEPS:"
echo "=================================="
echo ""
echo "1. Create GitHub repository:"
echo "   Go to: https://github.com/new"
echo "   Name: flarksiesis"
echo "   Description: Flarksiesis v2.0 - Advanced LFO-modulated filter effect by flarkAUDIO"
echo "   Make it: Public"
echo "   DO NOT add README, .gitignore, or license (we have them)"
echo ""
echo "2. After creating the repo, run these commands:"
echo ""
echo "   git remote add origin https://github.com/flarkflarkflark/flarksiesis.git"
echo "   git branch -M main"
echo "   git push -u origin main"
echo ""
echo "=================================="
echo ""
echo "Repository is ready to push!"
echo "Local path: $(pwd)"
echo ""
