#include "SliderComponent.h"

using namespace CMP;

SliderComponent::SliderComponent (Timecode& _current_time)
{
    Timecode& current_time = _current_time;
    timeSlider.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    timeSlider.setRange (0, 2);
}

void SliderComponent::SliderSetTime (Timecode& _current_time)
{
    timeSlider.setValue(SliderComponent::timeToDouble(_current_time));
}
SliderComponent::~SliderComponent (){}

double SliderComponent::timeToDouble (Timecode& _current_time) { return (double)3958.5; }

