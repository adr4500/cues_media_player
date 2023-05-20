#pragma once

#include <JuceHeader.h>
#include "Timecode.h"
#include "CueTypes.h"

namespace CMP
{

class ExternalInfo;

//------------------------------------------------------------------------------
/*
    This class represents a cue. This is what is displayed in the floating
   window. It contains a timecode, a name and a description.
*/
class Cue
{
public:
    //==============================================================================
    Cue (juce::String _timecode,
         CueTypes::CueType _cueType,
         juce::String _description);
    ~Cue ();

    //==============================================================================
    // Getters
    CMP::Timecode getTimecode () const;
    CueTypes::CueType getCueType () const;
    juce::String getDescription () const;
    bool isExecuted () const;

    //==============================================================================
    // Setters
    void setTimecode (CMP::Timecode _timecode);
    void setCueType (CueTypes::CueType _cueType);
    void setDescription (juce::String _description);
    void setExecuted (bool _executed);

    //==============================================================================
    // Other methods
    void executeCue (juce::MessageListener* _mainApp,
                     ExternalInfo* _externalInfo);

private:
    //==============================================================================
    CMP::Timecode timecode;
    CueTypes::CueType cueType;
    juce::String description;
    bool executed{false};
};

} // namespace CMP
