#include "MainComponent.h"
#include <cassert>

using namespace CMP;

juce::MessageListener* MainComponent::mainApplication = nullptr;

//==============================================================================
MainComponent::MainComponent (Timecode& _current_time,
                              ExternalInfo& _externalInfo)
    : current_time (_current_time),
      externalInfo (_externalInfo),
      gotoComponent (_externalInfo, mainApplication),
      settingsWindow (mainApplication, mtcSender)
{
    assert (mainApplication != nullptr);

    // Configure MTC Sender
    mtcSender.setCurrentTime (current_time);
    mtcSender.setTimeOffset (settingsWindow.getTimeOffset ());
    mtcSender.startThread ();

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
    addAndMakeVisible (settingsButton);
    settingsButton.setButtonText ("Settings");
    settingsButton.onClick = [this] () {
        // Open the settings window
        settingsWindow.open();
    };

    addAndMakeVisible (pausePlayButton);
    addAndMakeVisible (restartButton);
    restartButton.setButtonText ("Restart");
    restartButton.onClick = [this] () {
        CMP::ControlPannelMessage* restartMsg = new CMP::ControlPannelMessage (
            CMP::ControlPannelMessage::Type::Goto, "00:00:00:00");
        mainApplication->postMessage (restartMsg);
    };
    addAndMakeVisible (gotoComponent);

    // Configure Cues Components
    for (int i = 0; i < NB_DISPLAYED_TIMECODES; ++i)
    {
        cueComponents.emplace_back (new CueComponent (current_time));
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
    auto leftMargins = static_cast<int> (getWidth () * 0.02);
    auto topMargins = static_cast<int> (getHeight () * 0.02);

    auto buttonWidth = static_cast<int> (getWidth () / 2 - 2 * leftMargins);
    auto elemntWidth = static_cast<int> (getWidth () - 2 * leftMargins);
    auto elementsHeight = static_cast<int> (
        getHeight () / (NB_DISPLAYED_TIMECODES + 2.5) - 2 * topMargins);

    settingsButton.setBounds (leftMargins,
                             topMargins,
                             buttonWidth,
                             elementsHeight/2);
    pausePlayButton.setBounds (leftMargins,
                               topMargins * 2 + elementsHeight / 2,
                               buttonWidth,
                               elementsHeight);
    restartButton.setBounds (3 * leftMargins + buttonWidth,
                             topMargins * 2 + elementsHeight / 2,
                             buttonWidth,
                             elementsHeight);

    gotoComponent.setBounds (leftMargins,
                             4 * topMargins + elementsHeight * 1.5,
                             elemntWidth,
                             elementsHeight);

    for (int i = 0; i < NB_DISPLAYED_TIMECODES; ++i)
    {
        cueComponents[i]->setBounds (
            leftMargins,
            topMargins + (i + 2) * (elementsHeight + 4 * topMargins),
            elemntWidth,
            elementsHeight);
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
                mtcSender.start ();
            }
            else if (messagePtr->getMessage () == "Paused")
            {
                pausePlayButton.setButtonText ("Play");
                isVideoPlaying = false;
                mtcSender.stop ();
            }
            else if (messagePtr->getMessage () == "Timer")
            {
                for (auto& cueComponent : cueComponents)
                {
                    cueComponent->updateTime ();
                }
                if (cueComponents[nextCuePosition]->getCue ())
                {
                    if (cueComponents[nextCuePosition]
                                ->getCue ()
                                ->getTimecode () < current_time &&
                        cueComponents[nextCuePosition]
                                ->getCue ()
                                ->isExecuted () == false)
                    {
                        if (isVideoPlaying)
                        {
                            cueComponents[nextCuePosition]
                                ->getCue ()
                                ->executeCue (mainApplication, &externalInfo);
                        }
                        firstCueId++;
                        nextCuePosition++;
                        lastCueExecution = current_time;
                    }
                    /*
                    If there are too many "reached" cues, we remove them.
                    If the last reached cue is more than 5 seconds ago AND
                       the next cue is more than 5 seconds away, we remove the
                       reached cues.
                    */
                    if ((nextCuePosition >= NB_DISPLAYED_TIMECODES) ||
                        (lastCueExecution +
                                 Timecode (static_cast<uint64_t> (5) *
                                           1000000000) <
                             current_time &&
                         cueComponents[nextCuePosition]->getCue () &&
                         cueComponents[nextCuePosition]
                                     ->getCue ()
                                     ->getTimecode () -
                                 Timecode (static_cast<uint64_t> (5) *
                                           1000000000) >
                             current_time))
                    {
                        updateFirstCueId ();
                    }
                }
            }
            else if (messagePtr->getMessage () == "Cues")
            {
                updateFirstCueId ();
            }
        }
        else if (messagePtr->isType (ControlPannelMessage::Type::GotoOK))
        {
            refreshAllCues ();
            updateFirstCueId ();
            for (auto& cueComponent : cueComponents)
            {
                cueComponent->updateTime ();
            }
            mtcSender.sendMTC ();
        }
        else if (messagePtr->isType (ControlPannelMessage::Type::ReturnAudioTracks) ||
                 messagePtr->isType (ControlPannelMessage::Type::ReturnSubtitleTracks) ||
                 messagePtr->isType (ControlPannelMessage::Type::ReturnAudioCurrentTrack) ||
                 messagePtr->isType (ControlPannelMessage::Type::ReturnSubtitleCurrentTrack))
        {
            settingsWindow.handleMessage (_message);
        }
    }
}

void MainComponent::updateFirstCueId ()
{
    for (int i = 0; i < NB_DISPLAYED_TIMECODES; ++i)
    {
        auto* cueComponent = cueComponents[i];
        if (firstCueId + i < externalInfo.getCues ().size ())
        {
            cueComponent->setCue (&externalInfo.getCues ()[firstCueId + i]);
        }
        else
        {
            cueComponent->setCue (nullptr);
        }
        cueComponent->updateTime ();
    }
    nextCuePosition = 0;
}

void MainComponent::refreshAllCues ()
{
    firstCueId = -1;
    for (int i = 0; i < externalInfo.getCues ().size (); ++i)
    {
        if (externalInfo.getCues ()[i].getTimecode () > current_time)
        {
            externalInfo.getCues ()[i].setExecuted (false);
            if (firstCueId == -1)
            {
                firstCueId = i;
            }
        }
        else
        {
            externalInfo.getCues ()[i].setExecuted (true);
        }
    }
}
