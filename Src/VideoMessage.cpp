#include "VideoMessage.h"

using namespace CMP;

//==============================================================================
VideoMessage::VideoMessage (Type _type, juce::String _message)
    : type (_type), message (_message)
{
}

//==============================================================================
// Getters
VideoMessage::Type VideoMessage::getType () const { return type; }

bool VideoMessage::isType (Type _type) const { return type == _type; }

juce::String VideoMessage::getMessage () const { return message; }
