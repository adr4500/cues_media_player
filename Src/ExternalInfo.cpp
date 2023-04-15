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
void ExternalInfo::setVideoPath (juce::String path)
{
    videoPath = path;
    videoFile = juce::File (videoPath);
}

void ExternalInfo::setCSVPath (juce::String path)
{
    CSVPath = path;
    CSVFile = juce::File (CSVPath);
}

//==============================================================================
// Setup
bool ExternalInfo::setupCSV ()
{
    readCSV ();

    if (CSVLines.size () == 0)
        return false;

    for (int i = 0; i < CSVLines.size (); i++)
    {
        juce::StringArray line;

        // Split the line into 3 parts : timecode, name and description
        line.addTokens (CSVLines[i], ",", "\"");

        if (line.size () != 3)
            return false;

        Cue cue (line[0], line[1], line[2]);
        cues.push_back (cue);
    }

    sortCues ();

    return true;
}

void ExternalInfo::sortCues ()
{
    std::sort (cues.begin (), cues.end (), [](const Cue& a, const Cue& b) {
        return a.getTimecode () < b.getTimecode ();
    });
}

//==============================================================================
// CSV
void ExternalInfo::readCSV ()
{
    CSVLines.clear ();
    CSVFile.readLines (CSVLines);
}
