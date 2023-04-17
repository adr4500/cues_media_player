#include "CueComponent.h"

using namespace CMP;

//==============================================================================
// Circle Indicator
CircleIndicator::CircleIndicator () {}

CircleIndicator::~CircleIndicator () {}

void CircleIndicator::paint (juce::Graphics& g)
{
    const auto localBounds = getLocalBounds ().toFloat ();
    const auto circleDiameter =
        std::min (localBounds.getWidth (), localBounds.getHeight ()) * 0.5f;
    const juce::Rectangle<float> ellipseRectangle{
        0.f, 0.f, circleDiameter, circleDiameter};
    const auto justifiedEllipse =
        juce::Justification (juce::Justification::centred)
            .appliedToRectangle (ellipseRectangle, localBounds);
    g.setColour (fillColour);
    g.fillEllipse (justifiedEllipse);
}

void CircleIndicator::setColor (juce::Colour _color)
{
    color = _color;
}

void CircleIndicator::setStatus (bool _is_on)
{
    fillColour = _is_on ? color : juce::Colours::black;
}


//==============================================================================
// Constructor and Destructor
CueComponent::CueComponent (Timecode& _current_time)
    : current_time (_current_time)
{
    addAndMakeVisible (timecodeLabel);
    addAndMakeVisible (cueNameLabel);
    addAndMakeVisible (cueDescriptionLabel);
    timecodeLabel.setFont(juce::Font(20.0f));

    addAndMakeVisible (circleIndicators[0]);
    addAndMakeVisible (circleIndicators[1]);
    addAndMakeVisible (circleIndicators[2]);
    addAndMakeVisible (circleIndicators[3]);

    circleIndicators[0].setColor (juce::Colours::red);
    circleIndicators[1].setColor (juce::Colours::red);
    circleIndicators[2].setColor (juce::Colours::red);
    circleIndicators[3].setColor (juce::Colours::limegreen);
}

CueComponent::~CueComponent () {}

//==============================================================================
// Paint and Resize
void CueComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with
    // a solid colour)
    g.fillAll (getLookAndFeel ().findColour (
        juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
}

void CueComponent::resized ()
{
    auto leftMargin = static_cast<int> (getWidth () * 0.05);
    auto topMargin = static_cast<int> (getHeight () * 0.05);
    // Name and description
    auto labelWidth = static_cast<int> (getWidth () * 0.3);
    auto labelHeight = static_cast<int> (getHeight () * 0.4);
    cueNameLabel.setBounds (
        leftMargin, topMargin, labelWidth, labelHeight);
    cueDescriptionLabel.setBounds (
        leftMargin, 2 * topMargin + labelHeight, labelWidth, labelHeight);
    
    // Timecode
    auto timecodeWidth = static_cast<int> (getWidth () * 0.3);
    auto timecodeHeight = static_cast<int> (getHeight () * 0.9);
    timecodeLabel.setBounds (
        leftMargin + labelWidth, topMargin, timecodeWidth, timecodeHeight);
    
    // Circle Indicators
    auto space_between_circles = static_cast<int> (getWidth () * 0.01);
    auto circleWidth = static_cast<int> ((getWidth () * 0.3 - space_between_circles)/4);
    for (int i = 0; i < 4; i++)
    {
        circleIndicators[i].setBounds (
            leftMargin + labelWidth + timecodeWidth + space_between_circles + i * (circleWidth + space_between_circles),
            0,
            circleWidth,
            getHeight());
    }
}

//==============================================================================
// Set Cue
void CueComponent::setCue (Cue* _cue)
{
    cue = _cue;
}

//==============================================================================
// Update Time
void CueComponent::updateTime ()
{
    if (cue != nullptr)
    {
        auto time = cue->getTimecode () - current_time;
        auto timeString = time.toString ();
    }
}