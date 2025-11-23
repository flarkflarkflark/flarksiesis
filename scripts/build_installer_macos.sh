#!/bin/bash
# Build macOS installer for Flarksiesis

set -e

echo "Building Flarksiesis macOS Installer..."

# Build the plugin
mkdir -p build
cd build
cmake .. -G Xcode -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Create staging directory
STAGE_DIR="installer_stage"
rm -rf "$STAGE_DIR"
mkdir -p "$STAGE_DIR/VST3"
mkdir -p "$STAGE_DIR/Standalone"

# Copy built plugins
if [ -d "Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3" ]; then
    cp -R "Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3" "$STAGE_DIR/VST3/"
    echo "✓ VST3 copied"
fi

if [ -d "Flarksiesis_artefacts/Release/Standalone/Flarksiesis.app" ]; then
    cp -R "Flarksiesis_artefacts/Release/Standalone/Flarksiesis.app" "$STAGE_DIR/Standalone/"
    echo "✓ Standalone copied"
fi

# Ad-hoc sign for local use (remove quarantine)
echo "Signing plugins..."
codesign --force --deep --sign - "$STAGE_DIR/VST3/Flarksiesis.vst3" 2>/dev/null || true
codesign --force --deep --sign - "$STAGE_DIR/Standalone/Flarksiesis.app" 2>/dev/null || true

# Create installer package
echo "Creating installer package..."

# Create component plist
cat > component.plist << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<array>
    <dict>
        <key>BundleIsRelocatable</key>
        <false/>
        <key>BundleOverwriteAction</key>
        <string>upgrade</string>
        <key>RootRelativeBundlePath</key>
        <string>Library/Audio/Plug-Ins/VST3/Flarksiesis.vst3</string>
    </dict>
    <dict>
        <key>BundleIsRelocatable</key>
        <false/>
        <key>BundleOverwriteAction</key>
        <string>upgrade</string>
        <key>RootRelativeBundlePath</key>
        <string>Applications/flarkAUDIO/Flarksiesis.app</string>
    </dict>
</array>
</plist>
EOF

# Build component package
pkgbuild --root "$STAGE_DIR/VST3" \
         --identifier "com.flarkaudio.flarksiesis.vst3" \
         --version "2.0.0" \
         --install-location "/Library/Audio/Plug-Ins/VST3" \
         Flarksiesis-VST3.pkg

pkgbuild --root "$STAGE_DIR/Standalone" \
         --identifier "com.flarkaudio.flarksiesis.standalone" \
         --version "2.0.0" \
         --install-location "/Applications/flarkAUDIO" \
         Flarksiesis-Standalone.pkg

# Create distribution file
cat > distribution.xml << EOF
<?xml version="1.0" encoding="utf-8"?>
<installer-gui-script minSpecVersion="1">
    <title>Flarksiesis v2.0</title>
    <organization>com.flarkaudio</organization>
    <domains enable_localSystem="true"/>
    <options customize="always" require-scripts="false" hostArchitectures="x86_64,arm64"/>
    
    <welcome file="welcome.html"/>
    <license file="../LICENSE"/>
    <readme file="../README.md"/>
    
    <pkg-ref id="com.flarkaudio.flarksiesis.vst3"/>
    <pkg-ref id="com.flarkaudio.flarksiesis.standalone"/>
    
    <options customize="always" require-scripts="false"/>
    <choices-outline>
        <line choice="vst3"/>
        <line choice="standalone"/>
    </choices-outline>
    
    <choice id="vst3" title="VST3 Plugin" description="Install the VST3 plugin for use in DAWs">
        <pkg-ref id="com.flarkaudio.flarksiesis.vst3"/>
    </choice>
    <pkg-ref id="com.flarkaudio.flarksiesis.vst3" version="2.0.0" onConclusion="none">Flarksiesis-VST3.pkg</pkg-ref>
    
    <choice id="standalone" title="Standalone Application" description="Install the standalone Flarksiesis application">
        <pkg-ref id="com.flarkaudio.flarksiesis.standalone"/>
    </choice>
    <pkg-ref id="com.flarkaudio.flarksiesis.standalone" version="2.0.0" onConclusion="none">Flarksiesis-Standalone.pkg</pkg-ref>
</installer-gui-script>
EOF

# Create welcome message
cat > welcome.html << EOF
<!DOCTYPE html>
<html>
<head>
    <style>
        body { font-family: -apple-system, sans-serif; margin: 20px; }
        h1 { color: #FF6600; }
        .brand { color: #FF6600; font-weight: bold; }
    </style>
</head>
<body>
    <h1>Flarksiesis v2.0</h1>
    <p>Welcome to the <span class="brand">flarkAUDIO</span> Flarksiesis installer.</p>
    <p>This will install the Flarksiesis audio plugin on your Mac.</p>
    <h3>Features:</h3>
    <ul>
        <li>5 LFO waveform shapes</li>
        <li>8 filter types</li>
        <li>Tempo synchronization</li>
        <li>Stereo width control</li>
        <li>Real-time visualization</li>
    </ul>
</body>
</html>
EOF

# Build final installer
productbuild --distribution distribution.xml \
             --package-path . \
             --resources . \
             Flarksiesis-v2.0.0-macOS.pkg

echo ""
echo "✓ Installer created: Flarksiesis-v2.0.0-macOS.pkg"
echo ""
echo "To install: Open the .pkg file and follow the instructions"
