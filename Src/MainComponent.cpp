#include "MainComponent.h"
#include <cassert>

using namespace CMP;

juce::MessageListener* MainComponent::mainApplication = nullptr;

//==============================================================================
MainComponent::MainComponent (Timecode& _current_time) : current_time (_current_time)
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

    // Configure Cues Components
    for (int i = 0; i < NB_DISPLAYED_TIMECODES; ++i)
    {
        cueComponents.emplace_back (new CueComponent(current_time));
        addAndMakeVisible (cueComponents[i]);
    }

    setSize (600, 400);
}

MainComponent::~MainComponent ()
{
    for (auto& cueComponent : cueComponents)
    {
        delete cueComponent;
    }
    cueComponents.clear ();
}

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
    
    for (int i = 0; i < NB_DISPLAYED_TIMECODES; ++i)
    {
        auto cueComponentHeight = static_cast<int> (getHeight () * 0.15);
        auto cueComponentWidth = static_cast<int> (getWidth () * 0.96);
        auto cueComponentTopMargin = static_cast<int> (getHeight () * 0.02);
        auto cueComponentLeftMargin = static_cast<int> (getWidth () * 0.02);
        cueComponents[i]->setBounds (
            cueComponentLeftMargin,
            cueComponentTopMargin + buttonHeight + i * cueComponentHeight,
            cueComponentWidth,
            cueComponentHeight);
    }
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
            else if (messagePtr->getMessage () == "Paused")
            {
                pausePlayButton.setButtonText ("Play");
                isVideoPlaying = false;
            }
            else if (messagePtr->getMessage () == "Timer")
            {
                // TO DO
            }
        }
    }
}
