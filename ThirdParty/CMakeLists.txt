add_subdirectory(JUCE)

target_compile_definitions(${PROJECT_NAME} PUBLIC JUCE_WEB_BROWSER=0)
target_compile_definitions(${PROJECT_NAME} PUBLIC JUCE_USE_CURL=0)

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

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE juce::juce_audio_basics
          juce::juce_audio_devices
          juce::juce_audio_formats
          juce::juce_audio_processors
          juce::juce_audio_utils
          juce::juce_core
          juce::juce_data_structures
          juce::juce_dsp
          juce::juce_events
          juce::juce_graphics
          juce::juce_gui_basics
          juce::juce_gui_extra)
