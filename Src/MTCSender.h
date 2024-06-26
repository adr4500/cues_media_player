#pragma once

#include <JuceHeader.h>
#include "Timecode.h"

namespace CMP
{

//==============================================================================
/*
    This class is used to send MIDI Time Code (MTC) messages to a MIDI output.
    It sends the MTC messages at the frame rate of the video defined in
   Settings.h : FPS It is a new thread that sends the MTC messages at the right
   time.
*/
class MTCSender : public juce::Thread
{
public:
    //==============================================================================
    MTCSender ();
    ~MTCSender ();

    //==============================================================================
    void setMidiOutput (juce::String t_midiOutput);
    void setMidiOutputName (juce::String t_midiOutputName);
    void start ();
    void stop ();

    //==============================================================================
    juce::String getMidiOutputName ();

    void setCurrentTime (Timecode& t_currentTime)
    {
        currentTime = &t_currentTime;
    }

    void setTimeOffset (Timecode& t_timeOffset)
    {
        timeOffset = &t_timeOffset;
    }

    void run () override;

    void sendMTC ();

private:
    void sendFullMTC (Timecode t_time);
    void sendQuarterFrameMTC (int nibble, int value);
    juce::MidiOutput* midiOutput;
    bool isRunning = false;
    juce::MidiMessage::SmpteTimecodeType frameRate;
    Timecode lastTime;
    Timecode* currentTime;
    Timecode* timeOffset;
    Timecode codedTime;
    juce::String midiOutputName = "None";
    int sequence = 0;
};

} // namespace CMP
