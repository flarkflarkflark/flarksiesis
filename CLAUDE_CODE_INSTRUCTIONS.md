# Instructions for Claude Code

## Repository Setup and Push Instructions

This file contains everything you need to create and push the Flarksiesis v2.0 repository to GitHub.

### Repository Location
All files are located in: `/home/flark/Downloads/flarksiesis-repo/`

### Repository Structure
```
flarksiesis-repo/
├── .github/
│   └── workflows/
│       ├── linux.yml
│       ├── windows.yml
│       └── macos.yml
├── Source/
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   └── PluginEditor.cpp
├── .gitignore
├── .gitmodules
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## Step-by-Step Git Commands

### 1. Navigate to the repository directory
```bash
cd /home/flark/Downloads/flarksiesis-repo
```

### 2. Initialize Git repository
```bash
git init
```

### 3. Add all files to staging
```bash
git add .
```

### 4. Create initial commit
```bash
git commit -m "Initial commit: Flarksiesis v2.0

- Complete C++17 rewrite with JUCE framework
- 5 LFO waveform shapes (Sine, Triangle, Square, Saw, Random)
- 8 filter types with variable slopes
- Tempo synchronization support
- Stereo width and feedback controls
- Real-time LFO visualization
- Resizable orange-on-black UI
- Cross-platform GitHub Actions CI/CD
- Comprehensive documentation"
```

### 5. Add JUCE as a submodule
```bash
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
```

### 6. Commit the submodule addition
```bash
git add .gitmodules JUCE
git commit -m "Add JUCE framework as submodule"
```

### 7. Create GitHub repository
You'll need to create a repository on GitHub first. Options:
- Go to https://github.com/new
- Name it: `flarksiesis`
- Description: "Flarksiesis v2.0 - Advanced LFO-modulated filter effect by flarkAUDIO"
- Make it Public
- DO NOT initialize with README, .gitignore, or license (we already have these)

### 8. Add remote and push
Replace `YOUR_GITHUB_USERNAME` with your actual GitHub username:

```bash
git remote add origin https://github.com/YOUR_GITHUB_USERNAME/flarksiesis.git
git branch -M main
git push -u origin main
```

---

## Alternative: Using GitHub CLI

If you have GitHub CLI installed (`gh`):

```bash
cd /home/flark/Downloads/flarksiesis-repo

# Initialize and commit
git init
git add .
git commit -m "Initial commit: Flarksiesis v2.0"

# Add JUCE submodule
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
git add .gitmodules JUCE
git commit -m "Add JUCE framework as submodule"

# Create GitHub repo and push (will prompt for confirmation)
gh repo create flarksiesis --public --source=. --remote=origin --description="Flarksiesis v2.0 - Advanced LFO-modulated filter effect by flarkAUDIO"
git push -u origin main
```

---

## Post-Push Actions

### 1. Enable GitHub Actions
- Go to your repository on GitHub
- Click "Actions" tab
- Enable workflows if prompted

### 2. Add Repository Topics
Add these topics to make the repo discoverable:
- `audio-plugin`
- `vst3`
- `lv2`
- `juce`
- `cpp`
- `filter`
- `lfo`
- `modulation`
- `cross-platform`
- `music-production`

### 3. Create First Release (Optional)
To trigger the release build workflows:

```bash
git tag -a v2.0.0 -m "Release v2.0.0"
git push origin v2.0.0
```

Or via GitHub CLI:
```bash
gh release create v2.0.0 --title "Flarksiesis v2.0.0" --notes "Initial release of Flarksiesis v2.0"
```

---

## Verification Checklist

After pushing, verify:
- [ ] All files are present in the repository
- [ ] JUCE submodule is correctly linked
- [ ] README displays properly
- [ ] GitHub Actions workflows are visible
- [ ] License is correctly detected by GitHub

---

## Troubleshooting

### If push is rejected
```bash
git pull origin main --rebase
git push origin main
```

### If authentication fails
Make sure you have:
1. GitHub Personal Access Token (PAT) configured
2. Or SSH keys set up
3. Or GitHub CLI authenticated

### If submodule issues occur
```bash
git submodule deinit -f JUCE
rm -rf .git/modules/JUCE
git rm -f JUCE
# Then re-add the submodule
git submodule add https://github.com/juce-framework/JUCE.git JUCE
```

---

## Summary

All files are ready in `/home/flark/Downloads/flarksiesis-repo/`. 

The quickest path:
1. `cd /home/flark/Downloads/flarksiesis-repo`
2. `git init`
3. `git add .`
4. `git commit -m "Initial commit: Flarksiesis v2.0"`
5. `git submodule add https://github.com/juce-framework/JUCE.git JUCE`
6. `git add .gitmodules JUCE`
7. `git commit -m "Add JUCE submodule"`
8. Create repo on GitHub: https://github.com/new
9. `git remote add origin https://github.com/YOUR_USERNAME/flarksiesis.git`
10. `git push -u origin main`

Done! 🎉
