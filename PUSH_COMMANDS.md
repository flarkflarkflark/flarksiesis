# Commands for flarkflarkflark to Push to GitHub

## Method 1: Manual Push (Standard Git)

### Step 1: Initialize and Commit
```bash
cd /home/flark/Downloads/flarksiesis-repo

git init
git config user.name "flarkAUDIO"
git config user.email "your-email@example.com"  # ← Change this!

git add .
git commit -m "Initial commit: Flarksiesis v2.0.1 - macOS crash fix + installers"
```

### Step 2: Add JUCE Submodule
```bash
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
git add .gitmodules JUCE
git commit -m "Add JUCE framework as submodule"
```

### Step 3: Create GitHub Repository
1. Go to: https://github.com/new
2. Repository name: **flarksiesis**
3. Description: **Flarksiesis v2.0 - Advanced LFO-modulated filter effect by flarkAUDIO**
4. Choose: **Public**
5. **DO NOT** check any boxes (no README, no .gitignore, no license - we have them)
6. Click **"Create repository"**

### Step 4: Push to GitHub
```bash
git remote add origin https://github.com/flarkflarkflark/flarksiesis.git
git branch -M main
git push -u origin main
```

---

## Method 2: GitHub CLI (Easier - One Command!)

If you have GitHub CLI (`gh`) installed:

```bash
cd /home/flark/Downloads/flarksiesis-repo

# Initialize and commit
git init
git config user.name "flarkAUDIO"
git config user.email "your-email@example.com"
git add .
git commit -m "Initial commit: Flarksiesis v2.0.1"

# Add JUCE
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
git add .gitmodules JUCE
git commit -m "Add JUCE submodule"

# Create repo and push in one command!
gh repo create flarksiesis \
  --public \
  --source=. \
  --remote=origin \
  --description="Flarksiesis v2.0 - Advanced LFO-modulated filter effect by flarkAUDIO"

git push -u origin main
```

---

## All-in-One Copy-Paste (Manual Method)

```bash
cd /home/flark/Downloads/flarksiesis-repo && \
git init && \
git config user.name "flarkAUDIO" && \
git config user.email "your-email@example.com" && \
git add . && \
git commit -m "Initial commit: Flarksiesis v2.0.1 - macOS crash fix + installers" && \
git submodule add https://github.com/juce-framework/JUCE.git JUCE && \
git submodule update --init --recursive && \
git add .gitmodules JUCE && \
git commit -m "Add JUCE framework as submodule" && \
echo "" && \
echo "✓ Repository ready!" && \
echo "" && \
echo "Now create the repo on GitHub:" && \
echo "https://github.com/new" && \
echo "" && \
echo "Then run:" && \
echo "git remote add origin https://github.com/flarkflarkflark/flarksiesis.git" && \
echo "git branch -M main" && \
echo "git push -u origin main"
```

---

## After Pushing

Your repository will be live at:
**https://github.com/flarkflarkflark/flarksiesis**

GitHub Actions will automatically start building:
- ✅ Linux packages (.deb, .rpm, .tar.gz)
- ✅ macOS installer (.pkg)
- ✅ Windows installer (.exe)

Check the "Actions" tab after 10-15 minutes to download the builds!

---

## Troubleshooting

### If you get "permission denied":
You need to authenticate with GitHub. Either:
1. Use `gh auth login` (if using GitHub CLI)
2. Set up an SSH key
3. Use a Personal Access Token (PAT)

### If JUCE submodule fails:
```bash
git submodule deinit -f JUCE
rm -rf .git/modules/JUCE JUCE
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
```

### If push is rejected:
```bash
git pull origin main --rebase
git push origin main
```

---

## Your URLs

- **Repository**: https://github.com/flarkflarkflark/flarksiesis
- **Actions**: https://github.com/flarkflarkflark/flarksiesis/actions
- **Releases**: https://github.com/flarkflarkflark/flarksiesis/releases

---

Ready to go! 🚀
