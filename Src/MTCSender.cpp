#include "MTCSender.h"
#include "Settings.h"
#include <algorithm>

using namespace CMP;

//==============================================================================
MTCSender::MTCSender () : juce::Thread ("MTC Sender")
{
    midiOutput = nullptr;
    isRunning = false;
    if constexpr (FPS == 24)
    {
        frameRate = juce::MidiMessage::fps24;
    }
    else if constexpr (FPS == 25)
    {
        frameRate = juce::MidiMessage::fps25;
    }
    else if constexpr (FPS == 30)
    {
        frameRate = juce::MidiMessage::fps30;
    }
    else
    {
        frameRate = juce::MidiMessage::fps25;
    }
}

//==============================================================================
MTCSender::~MTCSender () { stop (); }

//==============================================================================
void MTCSender::setMidiOutput (juce::String t_midiOutput)
{
    if (t_midiOutput == "None" and midiOutput != nullptr)
    {
        midiOutput->stopBackgroundThread ();
        midiOutput = nullptr;
        return;
    }
    if (midiOutput != nullptr)
    {
        midiOutput->stopBackgroundThread ();
    }
    static std::unique_ptr<juce::MidiOutput> unique_midiOutput =
        juce::MidiOutput::openDevice (t_midiOutput);
    midiOutput = unique_midiOutput.get ();
    if (midiOutput != nullptr)
    {
        midiOutput->startBackgroundThread ();
    }
}

//==============================================================================
void MTCSender::setMidiOutputName (juce::String t_midiOutputName)
{
    midiOutputName = t_midiOutputName;
}

//==============================================================================
void MTCSender::start ()
{
    if (midiOutput != nullptr and currentTime != nullptr)
    {
        isRunning = true;
    }
}

//==============================================================================
void MTCSender::stop () { isRunning = false; }

//==============================================================================
juce::String MTCSender::getMidiOutputName () { return midiOutputName; }

//==============================================================================
void MTCSender::run ()
{
    // Calculate sleep time
    int sleepTime =
        1000 /
        (4 *
         FPS); // 4 times the frame rate because of the quarter frame messages
    while (!threadShouldExit ())
    {
        auto start = juce::Time::getMillisecondCounter ();
        if (isRunning and currentTime != nullptr)
        {
            sendMTC ();
        }
        auto end = juce::Time::getMillisecondCounter ();
        juce::Thread::sleep (std::max<int>(sleepTime - (int)(end - start), 0));
    }
}

//==============================================================================
void MTCSender::sendFullMTC (Timecode t_time)
{
    if (isRunning and midiOutput != nullptr)
    {
        juce::MidiMessage mtcMessage =
            juce::MidiMessage::fullFrame (t_time.getHours (),
                                          t_time.getMinutes (),
                                          t_time.getSeconds (),
                                          t_time.getFrames (),
                                          frameRate);
        midiOutput->sendMessageNow (mtcMessage);
    }
}

//==============================================================================
void MTCSender::sendQuarterFrameMTC (int nibble, int value)
{
    if (isRunning)
    {
        if (nibble == 7) // Nibble 7 is 0rrh, with r the rate, and h the hours
        {
            value = (frameRate << 1) | value;
        }
        juce::MidiMessage mtcMessage =
            juce::MidiMessage::quarterFrame (nibble, value);
        midiOutput->sendMessageNow (mtcMessage);
    }
}

void MTCSender::sendMTC ()
{
    if (midiOutput == nullptr or currentTime == nullptr)
    {
        return;
    }
    if (sequence == 0)
    {
        lastTime = codedTime;
        codedTime = *currentTime + *timeOffset;
        if (codedTime < Timecode ())
        {
            codedTime.setFramesTotal (0);
        }
    }
    static Timecode FullFrameDifference = Timecode ("00:00:00:10");
    if (codedTime - lastTime >= FullFrameDifference or
        codedTime - lastTime < Timecode ())
    {
        sendFullMTC (codedTime);
        sequence = 0;
    }
    else
    {
        auto nibbles = codedTime.toNibbles ();
        sendQuarterFrameMTC (sequence, nibbles[sequence]);
        sequence = (sequence + 1) % 8;
    }
}