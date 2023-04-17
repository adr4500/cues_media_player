#pragma once

#include <JuceHeader.h>

namespace CMP
{

//------------------------------------------------------------------------------
/*
    This class represents a timecode. It is used to store the timecode of a cue corresponding to the video.
*/
class Timecode
{
public:
    //==============================================================================
    Timecode(juce::String _timecode);
    ~Timecode();

    //==============================================================================
    // Getters
    int getHours() const;
    int getMinutes() const;
    int getSeconds() const;
    int getFrames() const;

    //==============================================================================
    // Setters
    void setHours(int _hours);
    void setMinutes(int _minutes);
    void setSeconds(int _seconds);
    void setFrames(int _frames);

    //==============================================================================
    // Operators
    bool operator> (const Timecode& other) const;
    bool operator< (const Timecode& other) const;
    bool operator== (const Timecode& other) const;
    bool operator!= (const Timecode& other) const;
    bool operator>= (const Timecode& other) const;
    bool operator<= (const Timecode& other) const;
     
private:
    //==============================================================================
    int hours;
    int minutes;
    int seconds;
    int frames;
};

} // namespace CMP
