#include "Timecode.h"
#include "Settings.h"

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
    frames = (nanoseconds / 41666667) % FPS;
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

    if (not isPositive)
        timecode += "-";

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

Timecode CMP::operator- (const Timecode& lhs, const Timecode& rhs)
{
    Timecode result;

    if (lhs < rhs)
    {
        result.isPositive = false;
        result.hours = rhs.hours - lhs.hours;
        result.minutes = rhs.minutes - lhs.minutes;
        result.seconds = rhs.seconds - lhs.seconds;
        result.frames = rhs.frames - lhs.frames;
    }
    else
    {
        result.isPositive = true;
        result.hours = lhs.hours - rhs.hours;
        result.minutes = lhs.minutes - rhs.minutes;
        result.seconds = lhs.seconds - rhs.seconds;
        result.frames = lhs.frames - rhs.frames;
    }

    if (result.frames < 0)
    {
        result.frames += FPS;
        result.seconds -= 1;
    }

    if (result.seconds < 0)
    {
        result.seconds += 60;
        result.minutes -= 1;
    }

    if (result.minutes < 0)
    {
        result.minutes += 60;
        result.hours -= 1;
    }

    return result;
}
