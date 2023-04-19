#pragma once

#include <JuceHeader.h>

namespace CMP
{

//==============================================================================
/*
    This class represents a message sent to Video. It is meant to be
   bidirectional.
*/
class ControlPannelMessage : public juce::Message
{
public:
    //==============================================================================
    enum class Type
    {
        None,

        TO_PANNEL = 1,
        Refresh,

        FROM_PANNEL = 100,
        Play,
        Pause,
        Restart,
        Goto,
        ErrorFromPannel,
    };

    //==============================================================================
    ControlPannelMessage (Type _type, juce::String _message = juce::String ());
    ~ControlPannelMessage () = default;

    //==============================================================================
    // Setters
    void setMessage (juce::String _message);

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
