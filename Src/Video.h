#pragma once

#include <gst/gst.h>
#include <JuceHeader.h>
#include "VideoMessage.h"

namespace CMP
{

//==============================================================================
/*
    This class represents a video. It manages all low level gstreamer calls.
*/
class Video : juce::MessageListener
{
public:
    //==============================================================================
    Video (const juce::File& _videoFile);
    ~Video () = default;

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

private:
    //==============================================================================
    // Setters
    void togglePlayPause ();
    void restartAndPause ();
    void terminate ();

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

    private:
        static gboolean
        busCallback (GstBus* _bus, GstMessage* _msg, gpointer _data);

        static void
        onPadAdded (GstElement* _element, GstPad* _pad, gpointer _data);
        static GstPadProbeReturn encoderCbHaveData (GstPad* pad,
                                                    GstPadProbeInfo* info,
                                                    gpointer user_data);

        void clean ();

        GstElement* pipeline{nullptr};
        GstElement* source{nullptr};
        GstElement* demuxer{nullptr};
        GstElement* parser{nullptr};
        GstElement* decoder{nullptr};
        GstElement* conv{nullptr};
        GstElement* sink{nullptr};

        GMainLoop* loop;

        GstBus* bus{nullptr};
        guint busWatchId{0};
        GstMessage* msg{nullptr};

        bool playing{false};

        juce::String videoPath;
    } videoThread;


    static juce::MessageListener* mainThread;
};

} // namespace CMP
