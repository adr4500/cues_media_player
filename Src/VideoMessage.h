#pragma once

#include <JuceHeader.h>

namespace CMP
{

//==============================================================================
/*
    This class represents a message sent to Video. It is meant to be
   bidirectional.
*/
class VideoMessage : public juce::Message
{
public:
    //==============================================================================
    enum class Type
    {
        None,

        TO_VIDEO = 1,

        PlayPause,
        Restart,
        Terminate,

        FROM_VIDEO = 100,
        ErrorFromVideo,
        EndOfStream,
        StateChanged
    };

    //==============================================================================
    VideoMessage (Type _type, juce::String _message = juce::String ());
    ~VideoMessage () = default;

    //==============================================================================
    // Getters
    Type getType () const;
    bool isType (Type _type) const;
    juce::String getMessage () const;

private:
    Type type;
    juce::String message;
};

} // namespace CMP
