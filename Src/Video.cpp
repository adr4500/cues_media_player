#include "Video.h"
#include "Settings.h"
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

long long int Video::getRunningTime () const { return videoThread.getRunningTime (); }

//==============================================================================
// VideoThread
void Video::VideoThread::run ()
{
    instance = new VLC::Instance (0, nullptr);
    player = new VLC::MediaPlayer (*instance);
    media = new VLC::Media (instance, videoPath);
    player->setMedia (*media);
    player->play ();
    player->pause ();
    playing = false;
    while (!threadShouldExit ())
    {
        juce::Thread::sleep (1000);
    }
    delete media;
    delete player;
    delete instance;
}

void Video::VideoThread::recieveMessage (const VideoMessage& _message)
{
    if (_message.isType (CMP::VideoMessage::Type::Play))
    {
        player->play ();
        playing = true;
        VideoMessage* returnMessage =
            new VideoMessage (VideoMessage::Type::StateChanged, "Playing");
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::Pause))
    {
        player->pause ();
        playing = false;
        VideoMessage* returnMessage =
            new VideoMessage (VideoMessage::Type::StateChanged, "Paused");
        Video::mainThread->postMessage (returnMessage);
    }
    else if (_message.isType (CMP::VideoMessage::Type::Restart))
    {
        player->setTime (0);
        player->pause ();
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
        long long int microsecondsRunningTime = static_cast<long long int> (
            _message.getMessage ().getLargeIntValue () * 1000 / FPS);
        player->setTime (microsecondsRunningTime);
        VideoMessage* returnMessage = new VideoMessage (
            VideoMessage::Type::GotoOK, _message.getMessage ());
        Video::mainThread->postMessage (returnMessage);
    }
}

void Video::VideoThread::stop ()
{
    player->stop ();
    playing = false;
}

bool Video::VideoThread::isPlaying () const { return playing; }

long long int Video::VideoThread::getRunningTime () const
{
    return player->time ();
}
