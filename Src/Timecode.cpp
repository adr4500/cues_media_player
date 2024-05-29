#include "Timecode.h"
#include "Settings.h"

using namespace CMP;

//==============================================================================

Timecode::Timecode (juce::String _timecode)
{
    int sign = 1;
    if (_timecode[0] == '-')
    {
        _timecode = _timecode.substring (1);
        sign = -1;
    }
    auto hours = _timecode.substring (0, 2).getIntValue ();
    auto minutes = _timecode.substring (3, 5).getIntValue ();
    auto seconds = _timecode.substring (6, 8).getIntValue ();
    auto frames = _timecode.substring (9, 11).getIntValue ();

    framesTotal =
        frames + seconds * FPS + minutes * 60 * FPS + hours * 3600 * FPS;
    
    framesTotal *= sign;
}

Timecode::~Timecode () {}

Timecode::Timecode (uint64_t nanoseconds)
{
    framesTotal = static_cast<int32_t> (nanoseconds * FPS / 1000000000);
}

//==============================================================================
// Getters
int Timecode::getHours () const
{
    return std::abs (framesTotal) / (3600 * FPS);
}

int Timecode::getMinutes () const
{
    return std::abs (framesTotal) / (60 * FPS) % 60;
}

int Timecode::getSeconds () const { return std::abs (framesTotal) / FPS % 60; }

int Timecode::getFrames () const { return std::abs (framesTotal) % FPS; }

int32_t Timecode::getFramesTotal () const { return framesTotal; }

juce::String Timecode::toString () const
{
    juce::String timecode;

    if (framesTotal < 0)
        timecode << "-";

    timecode << juce::String::formatted ("%02d", getHours ()) << ":"
             << juce::String::formatted ("%02d", getMinutes ()) << ":"
             << juce::String::formatted ("%02d", getSeconds ()) << ":"
             << juce::String::formatted ("%02d", getFrames ());

    return timecode;
}

float Timecode::toSeconds () const { return framesTotal / (float)FPS; }

std::array<int, 8> Timecode::toNibbles () const
{
    std::array<int, 8> nibbles;

    nibbles[0] = getFrames () & 0x0F;
    nibbles[1] = getFrames () >> 4;
    nibbles[2] = getSeconds () & 0x0F;
    nibbles[3] = getSeconds () >> 4;
    nibbles[4] = getMinutes () & 0x0F;
    nibbles[5] = getMinutes () >> 4;
    nibbles[6] = getHours () & 0x0F;
    nibbles[7] = getHours () >> 4;

    return nibbles;
}

//==============================================================================
// Setters
void Timecode::setFramesTotal (int32_t _framesTotal)
{
    framesTotal = _framesTotal;
}

//==============================================================================
// Operators
bool Timecode::operator> (const Timecode& other) const
{
    return framesTotal > other.framesTotal;
}

bool Timecode::operator< (const Timecode& other) const
{
    return framesTotal < other.framesTotal;
}

bool Timecode::operator== (const Timecode& other) const
{
    return framesTotal == other.framesTotal;
}

bool Timecode::operator!= (const Timecode& other) const
{
    return framesTotal != other.framesTotal;
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
    framesTotal = other.framesTotal;

    return *this;
}

Timecode CMP::operator- (const Timecode& lhs, const Timecode& rhs)
{
    Timecode result;

    result.framesTotal = lhs.framesTotal - rhs.framesTotal;

    return result;
}

Timecode CMP::operator+ (const Timecode& lhs, const Timecode& rhs)
{
    Timecode result;

    result.framesTotal = lhs.framesTotal + rhs.framesTotal;

    return result;
}
