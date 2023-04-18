#include <JuceHeader.h>

namespace CMP
{

//==============================================================================
/*
    This class is a singleton that stores all thge cues types.
*/
class CueTypes
{
public:
    //==============================================================================
    static CueTypes& getInstance ()
    {
        static CueTypes instance;
        return instance;
    }

    class CueType
    {
    public:
        CueType (juce::String _name, juce::Colour _colour)
            : name (_name), colour (_colour){};

        juce::String getName () const { return name; }
        juce::Colour getColour () const { return colour; }

        bool isCommandCue () const { return name == "Command"; }

    private:
        juce::String name;
        juce::Colour colour;
    };

    //==============================================================================
    void addCueType (juce::String _name)
    {
        // Pick a random light colour
        juce::Colour colour =
            juce::Colour::fromHSV (juce::Random ().nextFloat (),
                                   juce::Random ().nextFloat () * 0.5f + 0.5f,
                                   juce::Random ().nextFloat () * 0.3f + 0.7f,
                                   1.0f);

        cueTypes.emplace_back (CueType (_name, colour));
    }

    void addCueType (juce::String _name, juce::Colour _colour)
    {
        cueTypes.emplace_back (CueType (_name, _colour));
    }

    CueType getCueType (juce::String _name)
    {
        for (const auto& cueType : cueTypes)
        {
            if (cueType.getName () == _name)
                return cueType;
        }

        // If the cue type doesn't exist, create a new one and return it
        addCueType (_name);
        return getCueType (_name);
    }

private:
    //==============================================================================
    CueTypes ()
    {
        // Add the default cue types

        // Default cues
        addCueType ("Default", {255, 255, 255});
        // Command cues
        addCueType ("Command", {189, 121, 232});
        // GOTO cues
        addCueType ("GOTO", {0, 0, 0});
    }

    std::vector<CueType> cueTypes;
};

} // namespace CMP
