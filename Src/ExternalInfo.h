#pragma once

#include <JuceHeader.h>
#include "Cue.h"
#include "Settings.h"

inline bool isTimecodeFormat (const juce::String& _string)
{
    juce::String _string_copy = _string;
    if (_string[0]=='-')
    {
        _string_copy = _string.substring (1);
    }
    if (_string_copy.length () != 11)
        return false;

    if (_string_copy[2] != ':' || _string_copy[5] != ':' || _string_copy[8] != ':')
        return false;

    if (!_string_copy.substring (0, 2).containsOnly ("0123456789") ||
        !_string_copy.substring (3, 5).containsOnly ("0123456789") ||
        !_string_copy.substring (6, 8).containsOnly ("0123456789") ||
        !_string_copy.substring (9, 11).containsOnly ("0123456789"))
        return false;

    if (_string_copy.substring (3, 5).getIntValue () > 59 ||
        _string_copy.substring (6, 8).getIntValue () > 59 ||
        _string_copy.substring (9, 11).getIntValue () > FPS - 1)
        return false;

    return true;
}

namespace CMP
{

//==============================================================================
/*
    This class stores the data that is imported from external files : The CSV
   file that contains cues and the video.
*/
class ExternalInfo
{
public:
    //==============================================================================
    ExternalInfo ();
    ~ExternalInfo ();

    //==============================================================================
    // Getters
    juce::File& getVideoFile ();
    std::vector<Cue>& getCues ();
    std::vector<Cue>& getGotoCues ();

    //==============================================================================
    // Setters
    bool setVideoPath (void);
    bool setCSVPath (void);

    //==============================================================================
    // Setup
    bool setupCSV ();
    void sortCues ();

private:
    //==============================================================================
    // Private methods
    void readCSV ();

    //==============================================================================
    juce::File videoFile;
    juce::File CSVFile;
    juce::File lastdir;
    juce::StringArray CSVLines;

    std::vector<Cue> cues;
    std::vector<Cue> goto_cues;
};

} // namespace CMP
