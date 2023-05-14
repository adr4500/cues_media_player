#pragma once

#include <JuceHeader.h>
#include "ControlPannelMessage.h"
#include "Timecode.h"
#include "CueComponent.h"
#include "Settings.h"
#include "ExternalInfo.h"
#include "GotoComponent.h"
#include "SliderComponent.h"

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
    MainComponent (Timecode& _current_time, ExternalInfo& _externalInfo);
    ~MainComponent () override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized () override;

    //==============================================================================
    void handleMessage (const juce::Message& _message) override;

    void updateFirstCueId ();

    //==============================================================================
    // Static
    static void setMainApplication (juce::MessageListener* _mainApplication)
    {
        mainApplication = _mainApplication;
    }

private:
    void refreshAllCues ();
    static juce::MessageListener* mainApplication;

    bool isVideoPlaying{false};

    Timecode& current_time;
    ExternalInfo& externalInfo;

    Timecode lastCueExecution;


    juce::TextButton pausePlayButton;
    juce::TextButton restartButton;
    GotoComponent gotoComponent;

    std::vector<CueComponent*> cueComponents;
    int firstCueId{0};
    int nextCuePosition{0};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

} // namespace CMP
