#pragma once

#include <JuceHeader.h>
#include "ExternalInfo.h"

namespace CMP
{

//==============================================================================
/*
    This class represents the component that encapsulates the Goto behaviour,
   and the textbox and button components.
*/
class GotoComponent : public juce::Component
{
public:
    GotoComponent (ExternalInfo& _externalInfo,
                   juce::MessageListener* _mainApplication);
    ~GotoComponent ();

    void paint (juce::Graphics&) override;
    void resized () override;

private:
    void gotoTimecode (juce::String _timecode);

    juce::TextButton gotoButton;
    juce::TextEditor gotoTextBox;

    ExternalInfo& externalInfo;
    juce::MessageListener* mainApplication;
};

} // namespace CMP
