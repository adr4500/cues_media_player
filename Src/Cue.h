#pragma once

#include <JuceHeader.h>
#include "Timecode.h"

namespace CMP
{

//------------------------------------------------------------------------------
/*
    This class represents a cue. This is what is displayed in the floating
   window. It contains a timecode, a name and a description.
*/
class Cue
{
public:
    //==============================================================================
    Cue (juce::String _timecode, juce::String _name, juce::String _description);
    ~Cue ();

    //==============================================================================
    // Getters
    CMP::Timecode getTimecode () const;
    juce::String getName () const;
    juce::String getDescription () const;

    //==============================================================================
    // Setters
    void setTimecode (CMP::Timecode _timecode);
    void setName (juce::String _name);
    void setDescription (juce::String _description);

private:
    //==============================================================================
    CMP::Timecode timecode;
    juce::String name;
    juce::String description;
};

} // namespace CMP
