#include "Video.h"
#include <cassert>
#include <fstream>

using namespace CMP;

juce::MessageListener* Video::mainThread = nullptr;

//==============================================================================
Video::Video (const juce::File& _videoFile)
    : MessageListener (), videoThread (_videoFile.getFullPathName ())
{
    assert (mainThread != nullptr);
    videoThread.startThread ();
}

Video::~Video () { videoThread.stop (); }

//==============================================================================
// Getters
bool Video::isPlaying () const { return videoThread.isPlaying (); }

bool Video::isPaused () const { return !videoThread.isPlaying (); }

//==============================================================================
// MessageListener
void Video::handleMessage (const juce::Message& _message)
{
    auto messagePtr = dynamic_cast<const VideoMessage*> (&_message);
    if (messagePtr == nullptr)
    {
        return;
    }
    if (messagePtr->isType (CMP::VideoMessage::Type::PlayPause))
    {
        togglePlayPause ();
    }
    else if (messagePtr->isType (CMP::VideoMessage::Type::Restart))
    {
        restartAndPause ();
    }
    else if (messagePtr->isType (CMP::VideoMessage::Type::Terminate))
    {
        terminate ();
    }
}

//==============================================================================
// Static
void Video::setMainThread (juce::MessageListener* _mainThread)
{
    assert (_mainThread != nullptr);
    mainThread = _mainThread;
}

//==============================================================================
// Setters
void Video::togglePlayPause () {}

void Video::restartAndPause () {}

void Video::terminate () {}

//==============================================================================
// VideoThread
void Video::VideoThread::run ()
{
    gst_init (nullptr, nullptr);

    loop = g_main_loop_new (NULL, FALSE);

    // Create the elements
    pipeline = gst_pipeline_new ("video-decoder");
    source = gst_element_factory_make ("filesrc", "file-source");
    demux = gst_element_factory_make ("qtdemux", "demux");
    videoqueue = gst_element_factory_make ("queue", "video-queue");
    videoparser = gst_element_factory_make ("h264parse", "h264-parser");
    videodecoder = gst_element_factory_make ("nvh264sldec", "h264-decoder");
    videoconv = gst_element_factory_make ("videoconvert", "converter");
    videosink = gst_element_factory_make ("d3d11videosink", "video-output");
    audioqueue = gst_element_factory_make ("queue", "audio-queue");
    audioparser = gst_element_factory_make ("mpegaudioparse", "aac-parser");
    audiodecoder = gst_element_factory_make ("mpg123audiodec", "audio-decoder");
    audioconv = gst_element_factory_make ("audioconvert", "audio-converter");
    audioresample =
        gst_element_factory_make ("audioresample", "audio-resampler");
    audiosink = gst_element_factory_make ("wasapisink", "audio-output");

    if (!pipeline || !source || !demux || !videoqueue || !videoparser ||
        !videodecoder || !videoconv || !videosink || !audioqueue ||
        !audioparser || !audiodecoder || !audioconv || !audioresample ||
        !audiosink)
    {
        VideoMessage* message =
            new VideoMessage (VideoMessage::Type::ErrorFromVideo,
                              "One or more elements could not be created");
        Video::mainThread->postMessage (message);
        return;
    }

    // Set the source location
    g_object_set (
        G_OBJECT (source), "location", videoPath.toStdString ().c_str (), NULL);

    // Set fullscreen
    g_object_set(G_OBJECT(videosink), "fullscreen-toggle-mode", 2, NULL); // Toggle fulllscreen on Alt+Enter

    // Configure groups
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    busWatchId = gst_bus_add_watch (bus, busCallback, loop);
    gst_object_unref (bus);

    gst_bin_add_many (GST_BIN (pipeline),
                      source,
                      demux,
                      videoqueue,
                      videoparser,
                      videodecoder,
                      videoconv,
                      videosink,
                      audioqueue,
                      audioparser,
                      audiodecoder,
                      audioconv,
                      audioresample,
                      audiosink,
                      NULL);

    if (not gst_element_link (source, demux))
    {
        VideoMessage* message =
            new VideoMessage (VideoMessage::Type::ErrorFromVideo,
                              "Unable to link source and demuxer");
        Video::mainThread->postMessage (message);
        clean ();
        return;
    }

    if (not gst_element_link_many (
            videoqueue, videoparser, videodecoder, videoconv, videosink, NULL))
    {
        VideoMessage* message =
            new VideoMessage (VideoMessage::Type::ErrorFromVideo,
                              "Unable to link video elements");
        Video::mainThread->postMessage (message);
        clean ();
        return;
    }
    if (not gst_element_link_many (audioqueue,
                                   audioparser,
                                   audiodecoder,
                                   audioconv,
                                   audioresample,
                                   audiosink,
                                   NULL))
    {
        VideoMessage* message =
            new VideoMessage (VideoMessage::Type::ErrorFromVideo,
                              "Unable to link audio elements");
        Video::mainThread->postMessage (message);
        clean ();
        return;
    }

    separateFlows = new DemuxOutput (audioqueue, videoqueue);

    // Connect after demuxer

    g_signal_connect (
        demux, "pad-added", G_CALLBACK (onPadAdded), separateFlows);

    // Play
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    GstPad* pad = gst_element_get_static_pad (videosink, "sink");
    gst_pad_add_probe (
        pad, GST_PAD_PROBE_TYPE_BUFFER, encoderCbHaveData, NULL, NULL);
    gst_object_unref (pad);

    g_main_loop_run (loop);

    clean ();
}

void Video::VideoThread::recieveMessage (const VideoMessage& _message)
{
    GstStateChangeReturn state;
    if (_message.isType (CMP::VideoMessage::Type::PlayPause))
    {
        if (playing)
        {
            state = gst_element_set_state (pipeline, GST_STATE_PAUSED);
        }
        else
        {
            state = gst_element_set_state (pipeline, GST_STATE_PLAYING);
        }
        playing = !playing;
        if (state == GST_STATE_CHANGE_FAILURE)
        {
            VideoMessage* message = new VideoMessage (
                VideoMessage::Type::ErrorFromVideo,
                "Unable to set the pipeline to the playing state.");
            Video::mainThread->postMessage (message);
            stop ();
        }
    }
    else if (_message.isType (CMP::VideoMessage::Type::Restart))
    {
        state = gst_element_set_state (pipeline, GST_STATE_READY);
        if (state == GST_STATE_CHANGE_FAILURE)
        {
            VideoMessage* message = new VideoMessage (
                VideoMessage::Type::ErrorFromVideo,
                "Unable to set the pipeline to the ready state.");
            Video::mainThread->postMessage (message);
            stop ();
        }
        state = gst_element_set_state (pipeline, GST_STATE_PLAYING);
        if (state == GST_STATE_CHANGE_FAILURE)
        {
            VideoMessage* message = new VideoMessage (
                VideoMessage::Type::ErrorFromVideo,
                "Unable to set the pipeline to the playing state.");
            Video::mainThread->postMessage (message);
            stop ();
        }
    }
    else if (_message.isType (CMP::VideoMessage::Type::Terminate))
    {
        stop ();
    }
}

void Video::VideoThread::stop ()
{
    g_main_loop_quit (loop);
    clean ();
}

bool Video::VideoThread::isPlaying () const { return playing; }

gboolean Video::VideoThread::busCallback (GstBus* /*bus*/,
                                          GstMessage* msg,
                                          gpointer data)
{
    GMainLoop* loop = (GMainLoop*)data;
    VideoMessage* message = nullptr;
    switch (GST_MESSAGE_TYPE (msg))
    {
    case GST_MESSAGE_EOS:
        message =
            new VideoMessage (VideoMessage::Type::EndOfStream, "End of stream");
        Video::mainThread->postMessage (message);
        g_main_loop_quit (loop);
        break;

    case GST_MESSAGE_ERROR:
        gchar* debug;
        GError* error;

        gst_message_parse_error (msg, &error, &debug);
        g_free (debug);
        message =
            new VideoMessage (VideoMessage::Type::EndOfStream, error->message);
        Video::mainThread->postMessage (message);
        g_error_free (error);
        g_main_loop_quit (loop);
        break;

    default:
        break;
    }
    return TRUE;
}

void Video::VideoThread::onPadAdded (GstElement* /*element*/,
                                     GstPad* source_pad,
                                     gpointer data)
{
    DemuxOutput* demuxOutput = (DemuxOutput*)data;
    GstPad* sink_pad = nullptr;
    GstCaps* caps = gst_pad_get_current_caps (source_pad);
    GstStructure* structure = gst_caps_get_structure (caps, 0);
    const gchar* name = gst_structure_get_name (structure);
    if (g_str_has_prefix (name, "video"))
    {
        sink_pad =
            gst_element_get_static_pad (demuxOutput->videoFlowInput, "sink");
        GstPadLinkReturn res = gst_pad_link (source_pad, sink_pad);
        if (res != GST_PAD_LINK_OK)
        {
            VideoMessage* message = new VideoMessage (
                VideoMessage::Type::ErrorFromVideo,
                "Unable to link video pad : Error code " + juce::String (res));
            Video::mainThread->postMessage (message);
        }
    }
    else if (g_str_has_prefix (name, "audio"))
    {
        sink_pad =
            gst_element_get_static_pad (demuxOutput->audioFlowInput, "sink");
        GstPadLinkReturn res = gst_pad_link (source_pad, sink_pad);
        if (res != GST_PAD_LINK_OK)
        {
            VideoMessage* message = new VideoMessage (
                VideoMessage::Type::ErrorFromVideo,
                "Unable to link audio pad : Error code " + juce::String (res));
            Video::mainThread->postMessage (message);
        }
    }
    else
    {
        VideoMessage* message = new VideoMessage (
            VideoMessage::Type::ErrorFromVideo,
            "Unable to determine the type of the pad : " + std::string (name));
        Video::mainThread->postMessage (message);
    }
    gst_object_unref (sink_pad);
}

GstPadProbeReturn Video::VideoThread::encoderCbHaveData (GstPad* /*pad*/,
                                                         GstPadProbeInfo* info,
                                                         gpointer /*user_data*/)
{
    GstBuffer* buf = gst_pad_probe_info_get_buffer (info);
    GstMemory* bufMem = gst_buffer_get_memory (buf, 0);
    GstMapInfo bufInfo;

    gst_memory_map (bufMem, &bufInfo, GST_MAP_READ);

    // bufInfo.data, bufInfo.size will contain encoded image data as output of
    // out pipeline
    gst_memory_unmap (bufMem, &bufInfo);
    return GST_PAD_PROBE_OK;
}

void Video::VideoThread::clean ()
{
    if (not separateFlows)
    {
        delete separateFlows;
    }
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_source_remove (busWatchId);
    g_main_loop_unref (loop);
}
