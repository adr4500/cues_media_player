#pragma once

#include <JuceHeader.h>
#include "ControlPannelMessage.h"

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
    MainComponent ();
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

    juce::TextButton pausePlayButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

} // namespace CMP
