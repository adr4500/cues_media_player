#include "UIModules.h"

namespace CMP
{
namespace UIModules
{

/*------------------------ SectionSeparator ------------------------*/

SectionSeparator::SectionSeparator (juce::String _text)
{
    text.setText (_text, juce::dontSendNotification);
    text.setJustificationType (juce::Justification::left);
    text.setFont (juce::Font (fontSize));
    text.setColour (juce::Label::textColourId, juce::Colours::grey);

    addAndMakeVisible (text);
    line.setStart ({0, 0});
    line.setEnd ({0, 0});
}

SectionSeparator::SectionSeparator () : SectionSeparator ("")
{
}

SectionSeparator::~SectionSeparator ()
{
}

void SectionSeparator::paint (juce::Graphics& _g)
{
    _g.setColour (juce::Colours::grey);
    _g.drawLine (line, 1.0f);
}

void SectionSeparator::resized ()
{
    text.setBounds (0, 0, juce::Font (fontSize).getStringWidth (text.getText()), getHeight ());
    line.setStart ({(float)text.getX() + text.getWidth() + 5, (float)getHeight() / 2});
    line.setEnd ({(float)getWidth() - 5, (float)getHeight() / 2});
}

void SectionSeparator::setText (juce::String _text)
{
    text.setText (_text, juce::dontSendNotification);
    resized ();
}

void SectionSeparator::setWidth (int _width)
{
    setSize (_width, fontSize * 1.5);
}

} // namespace UIModules

} // namespace CMP
