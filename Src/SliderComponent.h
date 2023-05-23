#pragma once

#include <JuceHeader.h>
#include "ExternalInfo.h"

namespace CMP
{
class SliderComponent : public juce::Component
{
public:
    SliderComponent (Timecode& current_time);
    ~SliderComponent ();
    void SliderSetTime (Timecode& _current_time);
    static double timeToDouble (Timecode& _current_time);

private:
    juce::Slider timeSlider;
    Timecode& current_time;
};
}