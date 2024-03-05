#pragma once

#include <sys/types.h>
#include <vlc/vlc.h>
#include <JuceHeader.h>
#include "VideoMessage.h"

namespace CMP
{

//==============================================================================
/*
    This class represents a video. It manages all VLC related operations
*/
class Video : public juce::MessageListener
{
public:
    //==============================================================================
    Video (const juce::File& _videoFile);
    ~Video ();

    //==============================================================================
    // Getters
    bool isPlaying () const;
    bool isPaused () const;

    //==============================================================================
    // MessageListener
    void handleMessage (const juce::Message& _message) override;

    //==============================================================================
    // Static
    static void setMainThread (juce::MessageListener* _mainThread);

    //==============================================================================
    long long int getRunningTime () const;

private:
    class VideoThread : public juce::Thread
    {
    public:
        VideoThread (juce::String _videoPath)
            : Thread ("Video Thread"), videoPath (_videoPath)
        {
        }
        ~VideoThread () = default;

        void run () override;
        void recieveMessage (const VideoMessage& _message);

        void stop ();

        bool isPlaying () const;

        long long int getRunningTime () const;

    private:
        libvlc_instance_t* vlcInstance{nullptr};
        libvlc_media_player_t* vlcMediaPlayer{nullptr};

        bool playing{false};

        juce::String videoPath;
    } videoThread;


    static juce::MessageListener* mainThread;
};

} // namespace CMP
