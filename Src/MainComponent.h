#pragma once

#include <JuceHeader.h>
#include "ControlPannelMessage.h"
#include "Timecode.h"

namespace CMP
{

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component, public juce::MessageListener
{
public:
    //==============================================================================
    MainComponent (Timecode& _current_time);
    ~MainComponent () override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized () override;

    //==============================================================================
    void handleMessage (const juce::Message& _message) override;

    //==============================================================================
    // Static
    static void setMainApplication (juce::MessageListener* _mainApplication)
    {
        mainApplication = _mainApplication;
    }

private:
    static juce::MessageListener* mainApplication;

    bool isVideoPlaying{false};

    Timecode& current_time;

    juce::TextButton pausePlayButton;
    juce::Label timecodeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

} // namespace CMP
