#include "Cue.h"

using namespace CMP;

//==============================================================================
Cue::Cue (juce::String _timecode, juce::String _name, juce::String _description)
    : timecode (_timecode)
{
    name = _name;
    description = _description;
}

Cue::~Cue () {}

//==============================================================================
// Getters
Timecode Cue::getTimecode () const { return timecode; }

juce::String Cue::getName () const { return name; }

juce::String Cue::getDescription () const { return description; }

//==============================================================================
// Setters
void Cue::setTimecode (CMP::Timecode _timecode) { timecode = _timecode; }

void Cue::setName (juce::String _name) { name = _name; }

void Cue::setDescription (juce::String _description)
{
    description = _description;
}
