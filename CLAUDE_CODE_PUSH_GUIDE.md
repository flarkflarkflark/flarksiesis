# Guide for Claude Code to Push Flarksiesis to GitHub

**Copy this entire message to your Claude Code session in Konsole**

---

## Task for Claude Code

I need you to push the Flarksiesis repository to GitHub for user **flarkflarkflark**.

**Repository Location**: `/home/flark/Downloads/flarksiesis-repo/`

**Target GitHub URL**: `https://github.com/flarkflarkflark/flarksiesis.git`

---

## Step-by-Step Commands

Please execute these commands in sequence:

### 1. Navigate to repository
```bash
cd /home/flark/Downloads/flarksiesis-repo
```

### 2. Initialize git (if not already done)
```bash
git init
git config user.name "flarkAUDIO"
git config user.email "flarkaudio@example.com"
```

### 3. Stage and commit all files
```bash
git add .
git commit -m "Initial commit: Flarksiesis v2.0.1

- Fixed macOS crash (timer and thread-safety issues)
- Added professional installers (.pkg, .exe, .deb, .rpm)
- Complete C++17 rewrite with JUCE framework
- 5 LFO waveforms, 8 filter types
- Tempo sync, stereo width, feedback controls
- Cross-platform GitHub Actions CI/CD
- Orange-on-black flarkAUDIO branding"
```

### 4. Add JUCE as submodule
```bash
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
git add .gitmodules JUCE
git commit -m "Add JUCE framework as submodule"
```

### 5. Create GitHub repository
Before pushing, I need you to create the GitHub repository. You can either:

**Option A: Use GitHub CLI**
```bash
gh repo create flarksiesis \
  --public \
  --source=. \
  --remote=origin \
  --description="Flarksiesis v2.0 - Advanced LFO-modulated filter effect by flarkAUDIO"
```

**Option B: Create manually**
If you can't use `gh`, tell the user to:
1. Go to https://github.com/new
2. Repository name: `flarksiesis`
3. Description: `Flarksiesis v2.0 - Advanced LFO-modulated filter effect by flarkAUDIO`
4. Make it Public
5. Do NOT add README, .gitignore, or license
6. Click "Create repository"

Then continue with step 6.

### 6. Add remote and push
```bash
git remote add origin https://github.com/flarkflarkflark/flarksiesis.git
git branch -M main
git push -u origin main
```

---

## What to Do If You Encounter Errors

### Authentication Required
If git asks for authentication:

**For HTTPS (will prompt for credentials):**
```bash
# Git will ask for:
# Username: flarkflarkflark
# Password: (use Personal Access Token, not password)
```

The user needs to provide a GitHub Personal Access Token as the password.

**For SSH (if user has SSH keys set up):**
```bash
# Change remote to SSH
git remote remove origin
git remote add origin git@github.com:flarkflarkflark/flarksiesis.git
git push -u origin main
```

### Network Issues
If you can't access GitHub:
```bash
# Check connectivity
ping github.com
curl https://github.com

# If blocked, the user needs to push from their own terminal
```

### JUCE Submodule Fails
If the submodule command fails:
```bash
# Skip for now, can be added later
git rm -f JUCE
rm -rf .git/modules/JUCE
# Continue with push, tell user to add JUCE later
```

---

## After Successful Push

Tell the user:

✅ **Repository is live at**: https://github.com/flarkflarkflark/flarksiesis

✅ **GitHub Actions will start building** automatically (check Actions tab)

✅ **Builds will be available in 10-15 minutes**:
   - Linux: VST3, LV2, Standalone + .deb/.rpm packages
   - macOS: VST3, Standalone + .pkg installer
   - Windows: VST3, Standalone + .exe installer

✅ **All builds include the macOS crash fix**

---

## Summary

Execute the commands above to:
1. Initialize git repository
2. Commit all files
3. Add JUCE submodule
4. Create GitHub repository
5. Push everything to https://github.com/flarkflarkflark/flarksiesis

Report any errors you encounter and I'll help troubleshoot.

---

**End of instructions for Claude Code**
