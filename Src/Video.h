#pragma once

#include <gst/gst.h>
#include <JuceHeader.h>
#include "VideoMessage.h"

namespace CMP
{

struct DemuxOutput
{
    DemuxOutput (GstElement* _audioFlowInput, GstElement* _videoFlowInput)
        : audioFlowInput (_audioFlowInput), videoFlowInput (_videoFlowInput)
    {
    }
    GstElement* audioFlowInput{nullptr};
    GstElement* videoFlowInput{nullptr};
};

//==============================================================================
/*
    This class represents a video. It manages all low level gstreamer calls.
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
    guint64 getRunningTime () const;

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

        guint64 getRunningTime () const;

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
        GstElement* demux{nullptr};

        GstElement* videoqueue{nullptr};
        GstElement* videoparser{nullptr};
        GstElement* videodecoder{nullptr};
        GstElement* videoconv{nullptr};
        GstElement* videosink{nullptr};
        GstElement* audioqueue{nullptr};
        GstElement* audioparser{nullptr};
        GstElement* audiodecoder{nullptr};
        GstElement* audioconv{nullptr};
        GstElement* audioresample{nullptr};
        GstElement* audiosink{nullptr};

        GMainLoop* loop;

        DemuxOutput* separateFlows{nullptr};

        GstBus* bus{nullptr};
        guint busWatchId{0};
        GstMessage* msg{nullptr};

        bool playing{false};

        juce::String videoPath;
    } videoThread;


    static juce::MessageListener* mainThread;
};

} // namespace CMP
