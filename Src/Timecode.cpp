#include "Timecode.h"

using namespace CMP;

//==============================================================================

Timecode::Timecode (juce::String _timecode)
{
    hours = _timecode.substring (0, 2).getIntValue ();
    minutes = _timecode.substring (3, 5).getIntValue ();
    seconds = _timecode.substring (6, 8).getIntValue ();
    frames = _timecode.substring (9, 11).getIntValue ();
}

Timecode::~Timecode () {}

Timecode::Timecode (uint64_t nanoseconds)
{
    hours = static_cast<int>(nanoseconds / 3600000000000);
    minutes = (nanoseconds / 60000000000) % 60;
    seconds = (nanoseconds / 1000000000) % 60;
    frames = (nanoseconds / 41666667) % 25;
}

//==============================================================================
// Getters
int Timecode::getHours () const { return hours; }

int Timecode::getMinutes () const { return minutes; }

int Timecode::getSeconds () const { return seconds; }

int Timecode::getFrames () const { return frames; }

juce::String Timecode::toString () const
{
    juce::String timecode;

    if (hours < 10)
        timecode += "0";
    timecode += juce::String (hours) + ":";

    if (minutes < 10)
        timecode += "0";
    timecode += juce::String (minutes) + ":";

    if (seconds < 10)
        timecode += "0";
    timecode += juce::String (seconds) + ":";

    if (frames < 10)
        timecode += "0";
    timecode += juce::String (frames);

    return timecode;
}

//==============================================================================
// Setters
void Timecode::setHours (int _hours) { hours = _hours; }

void Timecode::setMinutes (int _minutes) { minutes = _minutes; }

void Timecode::setSeconds (int _seconds) { seconds = _seconds; }

void Timecode::setFrames (int _frames) { frames = _frames; }

//==============================================================================
// Operators
bool Timecode::operator> (const Timecode& other) const
{
    if (hours > other.hours)
        return true;
    else if (hours == other.hours)
    {
        if (minutes > other.minutes)
            return true;
        else if (minutes == other.minutes)
        {
            if (seconds > other.seconds)
                return true;
            else if (seconds == other.seconds)
            {
                if (frames > other.frames)
                    return true;
            }
        }
    }

    return false;
}

bool Timecode::operator< (const Timecode& other) const
{
    if (hours < other.hours)
        return true;
    else if (hours == other.hours)
    {
        if (minutes < other.minutes)
            return true;
        else if (minutes == other.minutes)
        {
            if (seconds < other.seconds)
                return true;
            else if (seconds == other.seconds)
            {
                if (frames < other.frames)
                    return true;
            }
        }
    }

    return false;
}

bool Timecode::operator== (const Timecode& other) const
{
    if (hours == other.hours && minutes == other.minutes &&
        seconds == other.seconds && frames == other.frames)
        return true;

    return false;
}

bool Timecode::operator!= (const Timecode& other) const
{
    if (not(*this == other))
        return true;

    return false;
}

bool Timecode::operator>= (const Timecode& other) const
{
    if (*this > other || *this == other)
        return true;

    return false;
}

bool Timecode::operator<= (const Timecode& other) const
{
    if (*this < other || *this == other)
        return true;

    return false;
}

Timecode& Timecode::operator= (const Timecode& other)
{
    hours = other.hours;
    minutes = other.minutes;
    seconds = other.seconds;
    frames = other.frames;

    return *this;
}
