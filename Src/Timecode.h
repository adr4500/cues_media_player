#pragma once

#include <JuceHeader.h>

namespace CMP
{

class Timecode;
Timecode operator- (const Timecode& lhs, const Timecode& rhs);

//------------------------------------------------------------------------------
/*
    This class represents a timecode. It is used to store the timecode of a cue
   corresponding to the video.
*/
class Timecode
{
public:
    //==============================================================================
    Timecode () = default;
    Timecode (juce::String _timecode);
    ~Timecode ();
    Timecode (const Timecode& other) = default;
    Timecode (uint64_t nanoseconds);

    //==============================================================================
    // Getters
    int getHours () const;
    int getMinutes () const;
    int getSeconds () const;
    int getFrames () const;

    juce::String toString () const;

    //==============================================================================
    // Setters
    void setHours (int _hours);
    void setMinutes (int _minutes);
    void setSeconds (int _seconds);
    void setFrames (int _frames);

    //==============================================================================
    // Operators
    bool operator> (const Timecode& other) const;
    bool operator< (const Timecode& other) const;
    bool operator== (const Timecode& other) const;
    bool operator!= (const Timecode& other) const;
    bool operator>= (const Timecode& other) const;
    bool operator<= (const Timecode& other) const;

    Timecode& operator= (const Timecode& other);

    //==============================================================================
    // Friends
    friend Timecode operator- (const Timecode& lhs, const Timecode& rhs);

private:
    //==============================================================================
    int hours;
    int minutes;
    int seconds;
    int frames;

    bool isPositive{true};
};

} // namespace CMP
