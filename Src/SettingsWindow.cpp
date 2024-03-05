#include "SettingsWindow.h"

using namespace CMP;

//==============================================================================
SettingsComponent::SettingsComponent (juce::TextButton& _audioTrackButton, juce::TextButton& _subtitleTrackButton, juce::TextButton& _midiDeviceButton)
    : audioTrackButton (_audioTrackButton),
      subtitleTrackButton (_subtitleTrackButton),
      midiDeviceButton (_midiDeviceButton)
{
}

//==============================================================================
SettingsComponent::~SettingsComponent ()
{}

//==============================================================================
void SettingsComponent::resized ()
{
    // Set the layout of the component
    audioTrackButton.setBounds (10, 10, 280, 40);
    subtitleTrackButton.setBounds (10, 60, 280, 40);
    midiDeviceButton.setBounds (10, 110, 280, 40);
}

//==============================================================================
SettingsWindow::SettingsWindow (juce::MessageListener* _mainApplication, CMP::MTCSender& _mtcSender)
    : juce::DocumentWindow ("CMP Settings",
                            juce::Colours::darkgrey,
                            juce::DocumentWindow::allButtons,
                            true),
      content (audioTrackButton, subtitleTrackButton, midiDeviceButton),
      mainApplication (_mainApplication),
      mtcSender (_mtcSender)
{

    // Set the content of the window
    centreWithSize (400, 300);
    content.setBounds (0, 0, 400, 300);
    setContentNonOwned (&content, true);
    content.setVisible(true);

    // Set the buttons
    audioTrackButton.setButtonText ("Audio Track. Current : Unavailabe");
    content.addAndMakeVisible (audioTrackButton);
    audioTrackButton.onClick = [this] () {
        // Open a dropdown menu to select the audio track
        if (audioTracks.size () == 0)
        {
            return;
        }
        juce::PopupMenu menu;
        for (int i = 0; i < audioTracks.size (); ++i)
        {
            menu.addItem (i + 1, audioTracks[i]);
        }
        int result = menu.showAt (&audioTrackButton);
        if (result != 0)
        {
            currentAudioTrack = result - 1;
            audioTrackButton.setButtonText ("Audio Track. Current : " + audioTracks[currentAudioTrack]);
            ControlPannelMessage* message = new ControlPannelMessage (ControlPannelMessage::Type::SetAudioTrack, juce::String(result - 1));
            mainApplication->postMessage (message);
        }
    };
    subtitleTrackButton.setButtonText ("Subtitle Track. Current : Unavailable");
    content.addAndMakeVisible (subtitleTrackButton);
    subtitleTrackButton.onClick = [this] () {
        // Open a dropdown menu to select the subtitle track
        if (subtitleTracks.size () == 0)
        {
            return;
        }
        juce::PopupMenu menu;
        for (int i = 0; i < subtitleTracks.size (); ++i)
        {
            menu.addItem (i + 1, subtitleTracks[i]);
        }
        int result = menu.showAt (&subtitleTrackButton);
        if (result != 0)
        {
            currentSubtitleTrack = result - 1;
            subtitleTrackButton.setButtonText ("Subtitle Track. Current : " + subtitleTracks[currentSubtitleTrack]);
            ControlPannelMessage* message = new ControlPannelMessage (ControlPannelMessage::Type::SetSubtitleTrack, juce::String(result - 1));
            mainApplication->postMessage (message);
        }
    };
    content.addAndMakeVisible (midiDeviceButton);
    midiDeviceButton.setButtonText ("Select Midi Device. Current : Unavailable");
    midiDeviceButton.onClick = [this] () {
        // Open a dropdown menu to select the midi device
        juce::PopupMenu menu;
        auto midiOutputs = juce::MidiOutput::getAvailableDevices ();
        menu.addItem (1, "None");
        for (int i = 0; i < midiOutputs.size (); ++i)
        {
            menu.addItem (i + 2, midiOutputs[i].name);
        }
        int result = menu.showAt (&midiDeviceButton); 
        if (result != 0)
        {
            if (result == 1)
            {
                mtcSender.setMidiOutput ("None");
                mtcSender.setMidiOutputName ("None");
                midiDeviceButton.setButtonText ("Select Midi Device. Current : None");
                return;
            }
            mtcSender.setMidiOutput (midiOutputs[result - 2].identifier);
            mtcSender.setMidiOutputName (midiOutputs[result - 2].name);
            midiDeviceButton.setButtonText ("Select Midi Device. Current : " +
                                            midiOutputs[result - 2].name);
        }
    };
    content.resized ();
}

//==============================================================================
SettingsWindow::~SettingsWindow () {}

//==============================================================================
void SettingsWindow::closeButtonPressed ()
{
    // Close the window without destroying it
    setVisible (false);
}

//==============================================================================
void SettingsWindow::open ()
{
    requestAudioCurrentTrack ();
    requestSubtitleCurrentTrack ();
    requestAudioTracks ();
    requestSubtitleTracks ();
    midiDeviceButton.setButtonText ("Select Midi Device. Current : " + mtcSender.getMidiOutputName ());
    // Open the window
    setVisible (true);
}

//==============================================================================
void SettingsWindow::requestAudioTracks ()
{
    // Send a message to the main application to request the audio tracks
    ControlPannelMessage* message = new ControlPannelMessage (ControlPannelMessage::Type::RequestAudioTracks);
    mainApplication->postMessage (message);
}

//==============================================================================
void SettingsWindow::requestSubtitleTracks ()
{
    // Send a message to the main application to request the subtitle tracks
    ControlPannelMessage* message = new ControlPannelMessage (ControlPannelMessage::Type::RequestSubtitleTracks);
    mainApplication->postMessage (message);
}

//==============================================================================
void SettingsWindow::requestAudioCurrentTrack ()
{
    // Send a message to the main application to request the current audio track
    ControlPannelMessage* message = new ControlPannelMessage (ControlPannelMessage::Type::RequestAudioCurrentTrack);
    mainApplication->postMessage (message);
}

//==============================================================================
void SettingsWindow::requestSubtitleCurrentTrack ()
{
    // Send a message to the main application to request the current subtitle track
    ControlPannelMessage* message = new ControlPannelMessage (ControlPannelMessage::Type::RequestSubtitleCurrentTrack);
    mainApplication->postMessage (message);
}

//==============================================================================
void SettingsWindow::handleMessage (const juce::Message& _message)
{
    // Handle the messages from the main application
    auto* messagePtr = dynamic_cast<const ControlPannelMessage*> (&_message);
    if (messagePtr)
    {
        if (messagePtr->isType (ControlPannelMessage::Type::ReturnAudioTracks))
        {
            // Set the audio tracks
            juce::StringArray tracks;
            tracks.addLines (messagePtr->getMessage ());
            audioTracks.clear ();
            for (int i = 0; i < tracks.size (); ++i)
            {
                audioTracks.push_back (tracks[i]);
            }
        }
        else if (messagePtr->isType (ControlPannelMessage::Type::ReturnSubtitleTracks))
        {
            // Set the subtitle tracks
            juce::StringArray tracks;
            tracks.addLines (messagePtr->getMessage ());
            subtitleTracks.clear ();
            for (int i = 0; i < tracks.size (); ++i)
            {
                subtitleTracks.push_back (tracks[i]);
            }
        }
        else if (messagePtr->isType (ControlPannelMessage::Type::ReturnAudioCurrentTrack))
        {
            // Set the current audio track
            auto currentAudioTrackString = messagePtr->getMessage ();
            int currentAudioTrackIndex = -1;
            for (int i = 0; i < audioTracks.size (); ++i)
            {
                // Remove the first argument of the audio track string (formatted xx:xx:xx)
                auto compareString = audioTracks[i].fromFirstOccurrenceOf (":", false, false);
                if (compareString.compare(currentAudioTrackString) == 0)
                {
                    currentAudioTrackIndex = i;
                    break;
                }
            }
            if (currentAudioTrackIndex != -1)
            {
                currentAudioTrack = currentAudioTrackIndex;
                audioTrackButton.setButtonText ("Audio Track. Current : " + audioTracks[currentAudioTrack]);
            }
            else
            {
                currentAudioTrack = -1;
                audioTrackButton.setButtonText ("Audio Track. Current : Unavailable");
            }
        }
        else if (messagePtr->isType (ControlPannelMessage::Type::ReturnSubtitleCurrentTrack))
        {
            // Set the current subtitle track
            auto currentSubtitleTrackString = messagePtr->getMessage ();
            int currentSubtitleTrackIndex = -1;
            for (int i = 0; i < subtitleTracks.size (); ++i)
            {
                // Remove the first argument of the subtitle track string (formatted xx:xx:xx)
                auto compareString = subtitleTracks[i].fromFirstOccurrenceOf (":", false, false);
                if (compareString.compare(currentSubtitleTrackString) == 0)
                {
                    currentSubtitleTrackIndex = i;
                    break;
                }
            }
            if (currentSubtitleTrackIndex != -1)
            {
                currentSubtitleTrack = currentSubtitleTrackIndex;
                subtitleTrackButton.setButtonText ("Subtitle Track. Current : " + subtitleTracks[currentSubtitleTrack]);
            }
            else
            {
                currentSubtitleTrack = -1;
                subtitleTrackButton.setButtonText ("Subtitle Track. Current : Unavailable");
            }
        }
    }
}
