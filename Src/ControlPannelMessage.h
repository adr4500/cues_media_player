#pragma once

#include <JuceHeader.h>
#include "ExternalInfo.h"

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
        GotoOK,
        ReturnAudioTracks,
        ReturnSubtitleTracks,
        ReturnAudioCurrentTrack,
        ReturnSubtitleCurrentTrack,

        FROM_PANNEL = 100,
        Play,
        Pause,
        Restart,
        Goto,
        RequestAudioTracks,
        RequestSubtitleTracks,
        RequestAudioCurrentTrack,
        RequestSubtitleCurrentTrack,
        SetAudioTrack,
        SetSubtitleTrack,
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

inline void gotoTimecode (juce::MessageListener* _mainApplication,
                          ExternalInfo* _externalInfo,
                          juce::String _gotoArgument)
{
    if (isTimecodeFormat (_gotoArgument))
    {
        Timecode timecode (_gotoArgument);
        ControlPannelMessage* message = new ControlPannelMessage (
            ControlPannelMessage::Type::Goto,
            juce::String (timecode.getFramesTotal ()));
        _mainApplication->postMessage (message);
    }
    else
    {
        for (auto gotoCue : _externalInfo->getGotoCues ())
        {
            if (gotoCue.getDescription () == _gotoArgument)
            {
                ControlPannelMessage* message = new ControlPannelMessage (
                    ControlPannelMessage::Type::Goto,
                    juce::String (gotoCue.getTimecode ().getFramesTotal ()));
                _mainApplication->postMessage (message);
                return;
            }
        }
    }
}

} // namespace CMP
