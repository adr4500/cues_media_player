#include "GotoComponent.h"
#include "ControlPannelMessage.h"

using namespace CMP;

GotoComponent::GotoComponent (ExternalInfo& _externalInfo,
                              juce::MessageListener* _mainApplication)
    : externalInfo (_externalInfo), mainApplication (_mainApplication)
{
    addAndMakeVisible (gotoButton);
    gotoButton.setButtonText ("Goto");
    gotoButton.onClick = [this] { gotoTimecode (mainApplication, &externalInfo, gotoTextBox.getText ()); };

    addAndMakeVisible (gotoTextBox);
    gotoTextBox.setTextToShowWhenEmpty ("Timecode name or timecode value",
                                        juce::Colour::fromRGB (172, 172, 172));
}

GotoComponent::~GotoComponent () {}

void GotoComponent::paint (juce::Graphics& /*g*/) {}

void GotoComponent::resized ()
{
    // 2% margins aroung every element
    // Text box on the left, 80% of the width, Button 20% of width

    int horizontalMargin = static_cast<int> (getWidth () * 0.02);
    int verticalMargin = static_cast<int> (getHeight () * 0.02);

    int textBoxWidth =
        static_cast<int> (getWidth () * 0.8) - 2 * horizontalMargin;
    int textBoxHeight = getHeight () - 2 * verticalMargin;

    int buttonWidth =
        static_cast<int> (getWidth () * 0.2) - 2 * horizontalMargin;
    int buttonHeight = getHeight () - 2 * verticalMargin;

    gotoTextBox.setBounds (
        horizontalMargin, verticalMargin, textBoxWidth, textBoxHeight);
    gotoButton.setBounds (getWidth () - 2 * horizontalMargin - buttonWidth,
                          verticalMargin,
                          buttonWidth,
                          buttonHeight);
}
