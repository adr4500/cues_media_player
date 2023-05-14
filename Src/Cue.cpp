#include "Cue.h"
#include "ControlPannelMessage.h"
#include "ExternalInfo.h"

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

bool Cue::isExecuted () const { return executed; }

//==============================================================================
// Setters
void Cue::setTimecode (CMP::Timecode _timecode) { timecode = _timecode; }

void Cue::setCueType (CueTypes::CueType _cueType) { cueType = _cueType; }

void Cue::setDescription (juce::String _description)
{
    description = _description;
}

void Cue::setExecuted (bool _executed) { executed = _executed; }

//==============================================================================
// Other methods
void Cue::executeCue (juce::MessageListener* _mainApp,
                      ExternalInfo* _externalInfo)
{
    executed = true;
    if (cueType.isCommandCue ())
    {
        //================ Add commands behaviour here
        if (description == "Pause")
        {
            CMP::ControlPannelMessage* pauseMsg =
                new CMP::ControlPannelMessage (
                    CMP::ControlPannelMessage::Type::Pause);
            _mainApp->postMessage (pauseMsg);
        }
        else if (description.startsWith ("Goto "))
        {
            gotoTimecode (_mainApp, _externalInfo, description.substring (5));
        }
    }
}
