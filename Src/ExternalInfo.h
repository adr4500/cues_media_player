#pragma once

#include <JuceHeader.h>
#include "Cue.h"

namespace CMP
{

//==============================================================================
/*
    This class stores the data that is imported from external files : The CSV file that contains cues and the video.
*/
class ExternalInfo
{
public:
    //==============================================================================
    ExternalInfo();
    ~ExternalInfo();

    //==============================================================================
    // Getters
    juce::File& getVideoFile();
    std::vector<Cue>& getCues();

    //==============================================================================
    // Setters
    bool setVideoPath(juce::String _path);
    bool setCSVPath(juce::String _path);

    //==============================================================================
    // Setup
    bool setupCSV();
    void sortCues();

private:
    //==============================================================================
    // Private methods
    void readCSV();

    //==============================================================================
    juce::String videoPath;
    juce::String CSVPath;
    juce::File videoFile;
    juce::File CSVFile;
    juce::StringArray CSVLines;

    std::vector<Cue> cues;
};

}
