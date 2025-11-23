cmake_minimum_required(VERSION 3.15)

project(Flarksiesis VERSION 2.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE
add_subdirectory(JUCE)

# Plugin formats to build
set(FORMATS
    VST3
    Standalone
)

# Check if we can build LV2
if(UNIX AND NOT APPLE)
    list(APPEND FORMATS LV2)
endif()

# Plugin settings
juce_add_plugin(Flarksiesis
    COMPANY_NAME "flarkAUDIO"
    BUNDLE_ID "com.flarkaudio.flarksiesis"
    PLUGIN_MANUFACTURER_CODE Flrk
    PLUGIN_CODE Flks
    FORMATS ${FORMATS}
    PRODUCT_NAME "Flarksiesis"
    
    # VST3 category
    VST3_CATEGORIES "Fx" "Filter" "Modulation"
    
    # Plugin characteristics
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_VERSION "2.0.0"
    
    # Description
    DESCRIPTION "Flarksiesis - LFO-modulated filter effect with multiple waveforms and filter types"
)

# Source files
target_sources(Flarksiesis
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

# Compile definitions
target_compile_definitions(Flarksiesis
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
)

# Link libraries
target_link_libraries(Flarksiesis
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Include directories
target_include_directories(Flarksiesis
    PRIVATE
        Source
)

# Compiler flags for optimization
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(Flarksiesis PRIVATE /O2)
    else()
        target_compile_options(Flarksiesis PRIVATE -O3 -march=native)
    endif()
endif()

# Install rules
if(APPLE)
    # macOS installation paths
    set(VST3_INSTALL_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/VST3" CACHE PATH "VST3 install directory")
    set(APP_INSTALL_DIR "/Applications/flarkAUDIO" CACHE PATH "Application install directory")
    
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3"
            DESTINATION "${VST3_INSTALL_DIR}"
            OPTIONAL)
    
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Flarksiesis_artefacts/Release/Standalone/Flarksiesis.app"
            DESTINATION "${APP_INSTALL_DIR}"
            OPTIONAL)
            
elseif(WIN32)
    # Windows installation paths
    set(VST3_INSTALL_DIR "$ENV{CommonProgramFiles}/VST3" CACHE PATH "VST3 install directory")
    set(APP_INSTALL_DIR "$ENV{ProgramFiles}/flarkAUDIO/Flarksiesis" CACHE PATH "Application install directory")
    
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3"
            DESTINATION "${VST3_INSTALL_DIR}"
            OPTIONAL)
    
    install(PROGRAMS "${CMAKE_BINARY_DIR}/Flarksiesis_artefacts/Release/Standalone/Flarksiesis.exe"
            DESTINATION "${APP_INSTALL_DIR}"
            OPTIONAL)
            
elseif(UNIX)
    # Linux installation paths
    set(VST3_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/vst3" CACHE PATH "VST3 install directory")
    set(LV2_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/lv2" CACHE PATH "LV2 install directory")
    set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Binary install directory")
    
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Flarksiesis_artefacts/Release/VST3/Flarksiesis.vst3"
            DESTINATION "${VST3_INSTALL_DIR}"
            OPTIONAL)
    
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Flarksiesis_artefacts/Release/LV2/Flarksiesis.lv2"
            DESTINATION "${LV2_INSTALL_DIR}"
            OPTIONAL)
    
    install(PROGRAMS "${CMAKE_BINARY_DIR}/Flarksiesis_artefacts/Release/Standalone/Flarksiesis"
            DESTINATION "${BIN_INSTALL_DIR}"
            OPTIONAL)
endif()

# Include packaging (optional)
include(cmake/Packaging.cmake OPTIONAL)
