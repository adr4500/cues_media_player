cmake_minimum_required(VERSION 3.24.1)

set(PROJECT_NAME "Cues_Media_Player")
file(STRINGS VERSION CURRENT_VERSION)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

add_executable(${PROJECT_NAME} Src/main.cpp)
include_directories(${PROJECT_NAME} Inc)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ThirdParty/JUCE/modules/juce_analytics
          ThirdParty/JUCE/modules/juce_audio_basics
          ThirdParty/JUCE/modules/juce_audio_devices
          ThirdParty/JUCE/modules/juce_audio_formats
          ThirdParty/JUCE/modules/juce_audio_plugin_client
          ThirdParty/JUCE/modules/juce_audio_processors
          ThirdParty/JUCE/modules/juce_audio_utils
          ThirdParty/JUCE/modules/juce_box2d
          ThirdParty/JUCE/modules/juce_core
          ThirdParty/JUCE/modules/juce_cryptography
          ThirdParty/JUCE/modules/juce_data_structures
          ThirdParty/JUCE/modules/juce_dsp
          ThirdParty/JUCE/modules/juce_events
          ThirdParty/JUCE/modules/juce_graphics
          ThirdParty/JUCE/modules/juce_gui_basics
          ThirdParty/JUCE/modules/juce_gui_extra
          ThirdParty/JUCE/modules/juce_opengl
          ThirdParty/JUCE/modules/juce_osc
          ThirdParty/JUCE/modules/juce_product_unlocking
          ThirdParty/JUCE/modules/juce_video)

add_subdirectory(ThirdParty)
