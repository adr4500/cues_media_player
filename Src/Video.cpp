#include "Video.h"
#include <cassert>

using namespace CMP;

//==============================================================================
Video::Video (const juce::File& _videoFile, juce::MessageListener* _mainThread)
    : MessageListener (), mainThread (_mainThread)
{
}

//==============================================================================
// Getters
bool Video::isPlaying () const { return playing; }

bool Video::isPaused () const { return !playing; }

//==============================================================================
// MessageListener
void Video::handleMessage (const juce::Message& _message)
{
    auto messagePtr = dynamic_cast<const VideoMessage*> (&_message);
    if (messagePtr == nullptr)
    {
        return;
    }
    if (messagePtr->isType (CMP::VideoMessage::Type::PlayPause))
    {
        togglePlayPause ();
    }
    else if (messagePtr->isType (CMP::VideoMessage::Type::Restart))
    {
        restartAndPause ();
    }
    else if (messagePtr->isType (CMP::VideoMessage::Type::Terminate))
    {
        terminate ();
    }
}

//==============================================================================
// Routine
void Video::routine () {}

void Video::handleGstMessage (GstMessage* _msg) {}

//==============================================================================
// Setters
void Video::togglePlayPause () {}

void Video::restartAndPause () {}

void Video::terminate () {}
