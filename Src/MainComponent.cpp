#include "MainComponent.h"
#include <cassert>

using namespace CMP;

juce::MessageListener* MainComponent::mainApplication = nullptr;

//==============================================================================
MainComponent::MainComponent ()
{
    assert (mainApplication != nullptr);

    // Configure PausePlay Button
    pausePlayButton.onClick = [this] () {
        if (isVideoPlaying)
        {
            CMP::ControlPannelMessage* pauseMsg =
                new CMP::ControlPannelMessage (
                    CMP::ControlPannelMessage::Type::Pause);
            mainApplication->postMessage (pauseMsg);
        }
        else
        {
            CMP::ControlPannelMessage* playMsg = new CMP::ControlPannelMessage (
                CMP::ControlPannelMessage::Type::Play);
            mainApplication->postMessage (playMsg);
        }
    };
    addAndMakeVisible (pausePlayButton);
    setSize (600, 400);
}

MainComponent::~MainComponent () {}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with
    // a solid colour)
    g.fillAll (getLookAndFeel ().findColour (
        juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
}

void MainComponent::resized ()
{
    auto leftMargin = static_cast<int> (getWidth () * 0.02);
    auto topMargin = static_cast<int> (getHeight () * 0.02);
    auto buttonWidth = static_cast<int> (getWidth () * 0.96);
    auto buttonHeight = static_cast<int> (getHeight () * 0.2);

    pausePlayButton.setBounds (
        leftMargin, topMargin, buttonWidth, buttonHeight);
}

//==============================================================================
void MainComponent::handleMessage (const juce::Message& _message)
{
    auto* messagePtr = dynamic_cast<const ControlPannelMessage*> (&_message);
    if (messagePtr)
    {
        if (messagePtr->isType (ControlPannelMessage::Type::Refresh))
        {
            if (messagePtr->getMessage () == "Playing")
            {
                pausePlayButton.setButtonText ("Pause");
                isVideoPlaying = true;
            }
            else
            {
                pausePlayButton.setButtonText ("Play");
                isVideoPlaying = false;
            }
        }
    }
}
