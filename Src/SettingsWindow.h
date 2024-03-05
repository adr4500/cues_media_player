#pragma once

#include <JuceHeader.h>
#include "ControlPannelMessage.h"
#include "MTCSender.h"
#include <vector>

namespace CMP
{

class SettingsComponent : public juce::Component
{
public :
    SettingsComponent (juce::TextButton& _audioTrackButton, juce::TextButton& _subtitleTrackButton, juce::TextButton& _midiDeviceButton);
    ~SettingsComponent () override;

    void resized () override;
private :
    juce::TextButton& audioTrackButton;
    juce::TextButton& subtitleTrackButton;
    juce::TextButton& midiDeviceButton;
};

//==============================================================================
/*
    This class is a window that allows the user to change the settings of the
    application : audio track, subtitle track, midi device, etc.
*/

class SettingsWindow : public juce::DocumentWindow, public juce::MessageListener
{
public:
    //==============================================================================
    SettingsWindow (juce::MessageListener* _mainApplication, CMP::MTCSender& _mtcSender);
    ~SettingsWindow () override;

    //==============================================================================
    void closeButtonPressed () override;
    void open ();

    void handleMessage (const juce::Message& _message) override;

private:
    void requestAudioTracks ();
    void requestSubtitleTracks ();
    void requestAudioCurrentTrack ();
    void requestSubtitleCurrentTrack ();

    std::vector<juce::String> audioTracks;
    std::vector<juce::String> subtitleTracks;
    int currentAudioTrack;
    int currentSubtitleTrack;

    CMP::SettingsComponent content;

    juce::TextButton audioTrackButton;
    juce::TextButton subtitleTrackButton;
    juce::TextButton midiDeviceButton;

    juce::MessageListener* mainApplication;
    CMP::MTCSender& mtcSender;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsWindow)
};

} // namespace CMP
