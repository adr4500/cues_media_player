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
    const char* font_option = "--freetype-font=Lato Regular";
    char** vlc_args = new char*[2];
    vlc_args[0] = new char[strlen(font_option) + 1];
    strcpy(vlc_args[0], font_option);
    vlc_args[1] = nullptr;
    int argc = 1;

    vlcInstance = libvlc_new (argc, vlc_args);
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
    else if (_message.isType (CMP::VideoMessage::Type::RequestAudioTracks))
    {
        libvlc_media_tracklist_t* trackList;
        libvlc_track_type_t type = libvlc_track_audio;
        trackList = libvlc_media_player_get_tracklist (vlcMediaPlayer, type, false);
        int nbTracks = libvlc_media_tracklist_count (trackList);
        juce::String message;
        for (int i = 0; i < nbTracks; ++i)
        {
            auto track = libvlc_media_tracklist_at(trackList, i);
            message += juce::String (i) + " : " + juce::String (track->i_id) + " : " + juce::String (track->psz_name) + "\n";
        }
        VideoMessage* returnMessage = new VideoMessage (
            VideoMessage::Type::ReturnAudioTracks, message);
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::RequestSubtitleTracks))
    {
        libvlc_media_tracklist_t* trackList;
        libvlc_track_type_t type = libvlc_track_text;
        trackList = libvlc_media_player_get_tracklist (vlcMediaPlayer, type, false);
        int nbTracks = libvlc_media_tracklist_count (trackList);
        juce::String message;
        for (int i = 0; i < nbTracks; ++i)
        {
            auto track = libvlc_media_tracklist_at(trackList, i);
            message += juce::String (i) + " : " + juce::String (track->i_id) + " : " + juce::String (track->psz_name) + "\n";
        }
        VideoMessage* returnMessage = new VideoMessage (
            VideoMessage::Type::ReturnSubtitleTracks, message);
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::RequestAudioCurrentTrack))
    {
        libvlc_media_track_t* track;
        libvlc_track_type_t type = libvlc_track_audio;
        track = libvlc_media_player_get_selected_track (vlcMediaPlayer, type);
        if (track == nullptr)
        {
            return;
        }
        juce::String message = juce::String (track->i_id) + " : " + juce::String (track->psz_name);
        VideoMessage* returnMessage = new VideoMessage (
            VideoMessage::Type::ReturnAudioCurrentTrack, message);
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::RequestSubtitleCurrentTrack))
    {
        libvlc_media_track_t* track;
        libvlc_track_type_t type = libvlc_track_text;
        track = libvlc_media_player_get_selected_track (vlcMediaPlayer, type);
        if (track == nullptr)
        {
            return;
        }
        juce::String message = juce::String (track->i_id) + " : " + juce::String (track->psz_name);
        VideoMessage* returnMessage = new VideoMessage (
            VideoMessage::Type::ReturnSubtitleCurrentTrack, message);
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::SetAudioTrack))
    {
        libvlc_media_track_t* track;
        libvlc_track_type_t type = libvlc_track_audio;
        int id = _message.getMessage ().getIntValue ();
        track = libvlc_media_tracklist_at(libvlc_media_player_get_tracklist (vlcMediaPlayer, type, false), id);
        libvlc_media_player_select_track (vlcMediaPlayer, track);
    }
    else if (_message.isType (CMP::VideoMessage::Type::SetSubtitleTrack))
    {
        libvlc_media_track_t* track;
        libvlc_track_type_t type = libvlc_track_text;
        int id = _message.getMessage ().getIntValue ();
        track = libvlc_media_tracklist_at(libvlc_media_player_get_tracklist (vlcMediaPlayer, type, false), id);
        libvlc_media_player_select_track (vlcMediaPlayer, track);
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
