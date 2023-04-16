#pragma once

#include <gst/gst.h>
#include <JuceHeader.h>
#include "VideoMessage.h"

namespace CMP
{

//==============================================================================
/*
    This class represents a video. It manages all low level gstreamer calls.
*/
class Video : juce::MessageListener
{
public:
    //==============================================================================
    Video (const juce::File& _videoFile, juce::MessageListener* _mainThread);
    ~Video () = default;

    //==============================================================================
    // Getters
    bool isPlaying () const;
    bool isPaused () const;

    //==============================================================================
    // MessageListener
    void handleMessage (const juce::Message& _message) override;

    //==============================================================================
    // Routine
    void routine ();
    void handleGstMessage (GstMessage* _msg);

private:
    //==============================================================================
    // Setters
    void togglePlayPause ();
    void restartAndPause ();
    void terminate ();

    bool playing{false};

    juce::MessageListener* mainThread{nullptr};
};

} // namespace CMP
