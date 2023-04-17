#include "ControlPannelMessage.h"

using namespace CMP;

//==============================================================================
ControlPannelMessage::ControlPannelMessage (Type _type, juce::String _message)
    : type (_type), message (_message)
{
}

//==============================================================================
// Getters
ControlPannelMessage::Type ControlPannelMessage::getType () const
{
    return type;
}

bool ControlPannelMessage::isType (Type _type) const { return type == _type; }

juce::String ControlPannelMessage::getMessage () const { return message; }
