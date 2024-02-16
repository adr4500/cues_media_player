#pragma once

#include <JuceHeader.h>
#include <array>

namespace CMP
{

class Timecode;
Timecode operator- (const Timecode& lhs, const Timecode& rhs);
Timecode operator+ (const Timecode& lhs, const Timecode& rhs);

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
    int32_t getFramesTotal () const;

    juce::String toString () const;
    float toSeconds () const;
    std::array<int,8> toNibbles () const;

    //==============================================================================
    // Setters
    void setFramesTotal (int32_t _framesTotal);

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
    friend Timecode operator+ (const Timecode& lhs, const Timecode& rhs);

private:
    //==============================================================================
    int32_t framesTotal{0};
};

} // namespace CMP
