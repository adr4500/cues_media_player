#include "Video.h"
#include "Settings.h"
#include <cassert>
#include <fstream>
#include <iostream>

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
    auto* messagePtr = dynamic_cast<const VideoMessage*> (&_message);
    if (messagePtr != nullptr)
    {
        videoThread.recieveMessage (*messagePtr);
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

long long int Video::getRunningTime () const
{
    return videoThread.getRunningTime ();
}

//==============================================================================
// VideoThread
void Video::VideoThread::run ()
{
    vlcInstance = libvlc_new (0, nullptr);
    vlcMediaPlayer = libvlc_media_player_new (vlcInstance);
    libvlc_media_t* media = libvlc_media_new_path (videoPath.toRawUTF8 ());
    libvlc_media_player_set_media (vlcMediaPlayer, media);
    libvlc_media_release (media);
    libvlc_media_player_play (vlcMediaPlayer);
    VideoMessage* returnMessage = new VideoMessage (
            VideoMessage::Type::StateChanged, "Playing");
    Video::mainThread->postMessage (returnMessage);
    playing = true;
}

void Video::VideoThread::recieveMessage (const VideoMessage& _message)
{
    if (_message.isType (CMP::VideoMessage::Type::Play))
    {
        libvlc_media_player_play (vlcMediaPlayer);
        playing = true;
        VideoMessage* returnMessage =
            new VideoMessage (VideoMessage::Type::StateChanged, "Playing");
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::Pause))
    {
        libvlc_media_player_pause (vlcMediaPlayer);
        playing = false;
        VideoMessage* returnMessage =
            new VideoMessage (VideoMessage::Type::StateChanged, "Paused");
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::Restart))
    {
        libvlc_media_player_set_time (vlcMediaPlayer, 0, false);
        libvlc_media_player_pause (vlcMediaPlayer);
        VideoMessage* returnMessage =
            new VideoMessage (VideoMessage::Type::StateChanged, "Paused");
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::Terminate))
    {
        stop ();
    }
    else if (_message.isType (CMP::VideoMessage::Type::Goto))
    {
        long long int millisecondsRunningTime = static_cast<long long int> (
            _message.getMessage ().getLargeIntValue () * 1000 / FPS);
        libvlc_media_player_set_time (vlcMediaPlayer, millisecondsRunningTime, false);
        VideoMessage* returnMessage = new VideoMessage (
            VideoMessage::Type::GotoOK, _message.getMessage ());
        Video::mainThread->postMessage (returnMessage);
    }
}

void Video::VideoThread::stop ()
{
    playing = false;
    libvlc_media_player_stop_async (vlcMediaPlayer);
    libvlc_media_player_release (vlcMediaPlayer);
    libvlc_release (vlcInstance);
    signalThreadShouldExit ();
}

bool Video::VideoThread::isPlaying () const { return playing; }

long long int Video::VideoThread::getRunningTime () const
{
    return libvlc_media_player_get_time (vlcMediaPlayer);
}
