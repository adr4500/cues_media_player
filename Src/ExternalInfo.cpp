#include "ExternalInfo.h"

using namespace CMP;

//==============================================================================
ExternalInfo::ExternalInfo ()
{
    videoPath = "";
    CSVPath = "";
}

ExternalInfo::~ExternalInfo () {}

//==============================================================================
// Getters
juce::File& ExternalInfo::getVideoFile () { return videoFile; }

std::vector<Cue>& ExternalInfo::getCues () { return cues; }

//==============================================================================
// Setters
bool ExternalInfo::setVideoPath (juce::String _path)
{
    videoPath = _path;
    videoFile =
        juce::File::getCurrentWorkingDirectory ().getChildFile (videoPath);

    if (videoFile.existsAsFile ())
        return true;
    else
        return false;
}

bool ExternalInfo::setCSVPath (juce::String _path)
{
    CSVPath = _path;
    CSVFile = juce::File::getCurrentWorkingDirectory ().getChildFile (CSVPath);

    if (CSVFile.existsAsFile ())
        return true;
    else
        return false;
}

//==============================================================================
// Setup
bool ExternalInfo::setupCSV ()
{
    readCSV ();

    if (CSVLines.size () == 0)
        return false;

    // Ignore the first line of description
    for (int i = 1; i < CSVLines.size (); i++)
    {
        juce::StringArray line;

        // Split the line into 3 parts : timecode, name and description
        line.addTokens (CSVLines[i], ",", "\"");

        if (line.size () != 3)
            return false;

        if (!isTimecodeFormat (line[0]))
            return false;

        // Find the cue type
        CueTypes::CueType cueType =
            CueTypes::getInstance ().getCueType (line[1]);

        Cue cue (line[0], cueType, line[2]);
        if (line[1] != "GOTO")
        {
            cues.push_back (cue);
        }
        else
        {
            goto_cues.push_back (cue);
        }
    }

    sortCues ();

    return true;
}

void ExternalInfo::sortCues ()
{
    std::sort (cues.begin (), cues.end (), [] (const Cue& a, const Cue& b) {
        return a.getTimecode () < b.getTimecode ();
    });
}

//==============================================================================
// Private methods
void ExternalInfo::readCSV ()
{
    CSVLines.clear ();
    CSVFile.readLines (CSVLines);
}
