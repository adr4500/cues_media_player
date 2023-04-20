#include "GotoComponent.h"
#include "ControlPannelMessage.h"

using namespace CMP;

GotoComponent::GotoComponent (ExternalInfo& _externalInfo,
                              juce::MessageListener* _mainApplication)
    : externalInfo (_externalInfo), mainApplication (_mainApplication)
{
    addAndMakeVisible (gotoButton);
    gotoButton.setButtonText ("Goto");
    gotoButton.onClick = [this] { gotoTimecode (gotoTextBox.getText ()); };

    addAndMakeVisible (gotoTextBox);
    gotoTextBox.setText ("Timecode name or timecode value");
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

void GotoComponent::gotoTimecode (juce::String _gotoArgument)
{
    if (isTimecodeFormat (_gotoArgument))
    {
        Timecode timecode (_gotoArgument);
        ControlPannelMessage* message = new ControlPannelMessage (
            ControlPannelMessage::Type::Goto,
            juce::String (timecode.getFramesTotal ()));
        mainApplication->postMessage (message);
    }
    else
    {
        for (auto gotoCue : externalInfo.getGotoCues ())
        {
            if (gotoCue.getDescription () == _gotoArgument)
            {
                ControlPannelMessage* message = new ControlPannelMessage (
                    ControlPannelMessage::Type::Goto,
                    juce::String (gotoCue.getTimecode ().getFramesTotal ()));
                mainApplication->postMessage (message);
                return;
            }
        }
    }
}