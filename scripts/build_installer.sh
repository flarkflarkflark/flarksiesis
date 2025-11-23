#!/bin/bash
# Build Flarksiesis installer for the current platform

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$SCRIPT_DIR/.."

cd "$PROJECT_ROOT"

echo "=================================="
echo "Flarksiesis Installer Builder"
echo "=================================="
echo ""

# Detect platform
if [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macOS"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="Linux"
else
    echo "Unsupported platform: $OSTYPE"
    exit 1
fi

echo "Platform: $PLATFORM"
echo ""

# Build the project first
echo "Building Flarksiesis..."
mkdir -p build
cd build

if [ "$PLATFORM" == "macOS" ]; then
    cmake .. -G Xcode -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DCMAKE_BUILD_TYPE=Release
else
    cmake .. -DCMAKE_BUILD_TYPE=Release
fi

cmake --build . --config Release
echo "✓ Build complete"
echo ""

# Create installer
echo "Creating installer..."

if [ "$PLATFORM" == "macOS" ]; then
    # macOS .pkg installer
    echo "Building macOS installer package..."
    
    mkdir -p installer_stage/VST3 installer_stage/Standalone
    
    # Copy plugins
    if [ -d "Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3" ]; then
        cp -R "Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3" installer_stage/VST3/
        echo "  ✓ VST3 staged"
    fi
    
    if [ -d "Flarksiesis_artefacts/Release/Standalone/Flarksiesis.app" ]; then
        cp -R "Flarksiesis_artefacts/Release/Standalone/Flarksiesis.app" installer_stage/Standalone/
        echo "  ✓ Standalone staged"
    fi
    
    # Ad-hoc sign
    echo "  Signing..."
    codesign --force --deep --sign - installer_stage/VST3/Flarksiesis.vst3 2>/dev/null || true
    codesign --force --deep --sign - installer_stage/Standalone/Flarksiesis.app 2>/dev/null || true
    
    # Build component packages
    pkgbuild --root installer_stage/VST3 \
             --identifier com.flarkaudio.flarksiesis.vst3 \
             --version 2.0.0 \
             --install-location /Library/Audio/Plug-Ins/VST3 \
             Flarksiesis-VST3.pkg
    
    pkgbuild --root installer_stage/Standalone \
             --identifier com.flarkaudio.flarksiesis.standalone \
             --version 2.0.0 \
             --install-location /Applications/flarkAUDIO \
             Flarksiesis-Standalone.pkg
    
    # Create distribution
    cat > distribution.xml << 'EOF'
<?xml version="1.0" encoding="utf-8"?>
<installer-gui-script minSpecVersion="1">
    <title>Flarksiesis v2.0</title>
    <organization>com.flarkaudio</organization>
    <domains enable_localSystem="true"/>
    <options customize="always" require-scripts="false" hostArchitectures="x86_64,arm64"/>
    <pkg-ref id="com.flarkaudio.flarksiesis.vst3"/>
    <pkg-ref id="com.flarkaudio.flarksiesis.standalone"/>
    <choices-outline>
        <line choice="vst3"/>
        <line choice="standalone"/>
    </choices-outline>
    <choice id="vst3" title="VST3 Plugin" description="Install VST3 plugin for DAWs">
        <pkg-ref id="com.flarkaudio.flarksiesis.vst3"/>
    </choice>
    <pkg-ref id="com.flarkaudio.flarksiesis.vst3" version="2.0.0">Flarksiesis-VST3.pkg</pkg-ref>
    <choice id="standalone" title="Standalone App" description="Install standalone application">
        <pkg-ref id="com.flarkaudio.flarksiesis.standalone"/>
    </choice>
    <pkg-ref id="com.flarkaudio.flarksiesis.standalone" version="2.0.0">Flarksiesis-Standalone.pkg</pkg-ref>
</installer-gui-script>
EOF
    
    # Build final installer
    productbuild --distribution distribution.xml \
                 --package-path . \
                 Flarksiesis-v2.0.0-macOS.pkg
    
    echo ""
    echo "✓ Installer created: build/Flarksiesis-v2.0.0-macOS.pkg"
    echo ""
    echo "To install: Double-click the .pkg file"
    
elif [ "$PLATFORM" == "Linux" ]; then
    # Linux packages (.deb, .rpm, .tar.gz)
    echo "Building Linux packages..."
    
    # Use CPack if available
    if command -v cpack &> /dev/null; then
        cpack -G "DEB;RPM;TGZ"
        echo ""
        echo "✓ Packages created in: build/"
        ls -lh Flarksiesis*.{deb,rpm,tar.gz} 2>/dev/null || true
    else
        echo "CPack not found, creating tarball instead..."
        mkdir -p Flarksiesis-v2.0.0-Linux
        cp -r Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3 Flarksiesis-v2.0.0-Linux/ || true
        cp -r Flarksiesis_artefacts/Release/LV2/Flarksiesis.lv2 Flarksiesis-v2.0.0-Linux/ || true
        cp Flarksiesis_artefacts/Release/Standalone/Flarksiesis Flarksiesis-v2.0.0-Linux/ || true
        tar -czf Flarksiesis-v2.0.0-Linux.tar.gz Flarksiesis-v2.0.0-Linux/
        echo ""
        echo "✓ Package created: build/Flarksiesis-v2.0.0-Linux.tar.gz"
    fi
fi

echo ""
echo "=================================="
echo "Build complete!"
echo "=================================="
