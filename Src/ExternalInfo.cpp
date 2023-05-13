#include "ExternalInfo.h"

using namespace CMP;

//==============================================================================
ExternalInfo::ExternalInfo () {}

ExternalInfo::~ExternalInfo () {}

//==============================================================================
// Getters
juce::File& ExternalInfo::getVideoFile () { return videoFile; }

std::vector<Cue>& ExternalInfo::getCues () { return cues; }

std::vector<Cue>& ExternalInfo::getGotoCues () { return goto_cues; }

//==============================================================================
// Setters
bool ExternalInfo::setCSVPath (void)
{
    juce::FileChooser csvFinder ((juce::String) "Choisissez un fichier CSV ...",
                                 juce::File::getCurrentWorkingDirectory (),
                                 (juce::String) "*.csv");

    if (csvFinder.browseForFileToOpen ())
    {
        CSVFile = csvFinder.getResult ();
        lastdir = CSVFile.getParentDirectory ();
        return true;
    }
    else
        return false;
}

bool ExternalInfo::setVideoPath (void)
{
    juce::FileChooser videoFinder (
        (juce::String) "Choisissez un fichier video ...",
        lastdir,
        (juce::String) "*.mp4");
    if (videoFinder.browseForFileToOpen ())
    {
        videoFile = videoFinder.getResult ();
        return true;
    }
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
