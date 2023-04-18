#include "Cue.h"

using namespace CMP;

//==============================================================================
Cue::Cue (juce::String _timecode,
          CueTypes::CueType _cueType,
          juce::String _description)
    : timecode (_timecode), cueType (_cueType), description (_description)
{
}

Cue::~Cue () {}

//==============================================================================
// Getters
Timecode Cue::getTimecode () const { return timecode; }

CueTypes::CueType Cue::getCueType () const { return cueType; }

juce::String Cue::getDescription () const { return description; }

//==============================================================================
// Setters
void Cue::setTimecode (CMP::Timecode _timecode) { timecode = _timecode; }

void Cue::setCueType (CueTypes::CueType _cueType) { cueType = _cueType; }

void Cue::setDescription (juce::String _description)
{
    description = _description;
}
