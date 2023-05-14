#pragma once

#include <JuceHeader.h>
#include "ExternalInfo.h"

namespace CMP
{
class SliderComponent : public juce::Component
{
public:
    SliderComponent ();
    ~SliderComponent ();

private:
    juce::Slider timeSlider;
};
}