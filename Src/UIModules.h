#pragma once

#include <JuceHeader.h>

namespace CMP
{

namespace UIModules
{

class SectionSeparator : public juce::Component
{
public:
    SectionSeparator (juce::String _text);
    SectionSeparator ();
    ~SectionSeparator () override;

    void paint (juce::Graphics& _g) override;
    void resized () override;
    void setText (juce::String _text);
    void setWidth (int _width);

private:
    juce::Label text;
    juce::Line<float> line;
    float fontSize = 20.0f;
};

} // namespace UIModules

} // namespace CMP
