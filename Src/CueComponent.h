#pragma once

#include <JuceHeader.h>
#include "Cue.h"

namespace CMP
{

//==============================================================================
/*
    This component is a visual indicator of the cue proximity.
*/
class CircleIndicator : public juce::Component
{
public:
    //==============================================================================
    CircleIndicator ();
    ~CircleIndicator () override;

    //==============================================================================
    void paint (juce::Graphics&) override;

    //==============================================================================
    void setColor (juce::Colour _color);
    void setStatus (bool _is_on);

private:
    juce::Colour color;

    juce::Colour fillColour{juce::Colours::black};
};


/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class CueComponent : public juce::Component
{
public:
    //==============================================================================
    CueComponent (Timecode& _current_time);
    ~CueComponent () override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized () override;

    //==============================================================================
    void setCue (Cue* _cue);
    void updateTime ();

    //==============================================================================
    Cue* getCue ();

    //==============================================================================
private:
    Cue* cue{nullptr};
    Timecode& current_time;

    // Graphic Components
    juce::Label timecodeLabel;
    juce::Label cueNameLabel;
    juce::Label cueDescriptionLabel;

    std::array<CircleIndicator, 4> circleIndicators;
};

} // namespace CMP
