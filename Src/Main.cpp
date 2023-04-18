/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
#include "Video.h"
#include <cassert>

//==============================================================================
class CMPApplication : public juce::JUCEApplication,
                       public juce::MessageListener
{
public:
    //==============================================================================
    CMPApplication () {}

    const juce::String getApplicationName () override
    {
        return ProjectInfo::projectName;
    }
    const juce::String getApplicationVersion () override
    {
        return ProjectInfo::versionString;
    }
    bool moreThanOneInstanceAllowed () override { return false; }

    void handleMessage (const juce::Message& _message) override
    {
        if (auto* videoMessagePtr =
                dynamic_cast<const CMP::VideoMessage*> (&_message))
        {
            handleVideoMessage (*videoMessagePtr);
        }
        else if (auto* controlPannelMessagePtr =
                     dynamic_cast<const CMP::ControlPannelMessage*> (&_message))
        {
            handleControlPannelMessage (*controlPannelMessagePtr);
        }
        else
        {
            assert (false);
        }
    }

    void handleVideoMessage (const CMP::VideoMessage& _message)
    {
        switch (_message.getType ())
        {
        case CMP::VideoMessage::Type::ErrorFromVideo:
        {
            CMP::VideoMessage* terminateMsg =
                new CMP::VideoMessage (CMP::VideoMessage::Type::Terminate);
            video->postMessage (terminateMsg);
            juce::AlertWindow::showMessageBoxAsync (
                juce::AlertWindow::AlertIconType::WarningIcon,
                "Video Player Error",
                _message.getMessage (),
                "OK",
                nullptr,
                juce::ModalCallbackFunction::create ([] (int) {
                    juce::MessageManager::getInstance ()->stopDispatchLoop ();
                }));
            break;
        }
        case CMP::VideoMessage::Type::EndOfStream:
            juce::AlertWindow::showMessageBoxAsync (
                juce::AlertWindow::AlertIconType::InfoIcon,
                "Video Player",
                "The video has ended",
                "OK",
                nullptr,
                juce::ModalCallbackFunction::create ([] (int) {
                    juce::MessageManager::getInstance ()->stopDispatchLoop ();
                }));
            break;
        case CMP::VideoMessage::Type::StateChanged:
        {
            CMP::ControlPannelMessage* stateChangedMsg =
                new CMP::ControlPannelMessage (
                    CMP::ControlPannelMessage::Type::Refresh,
                    _message.getMessage ());
            controlPannelWindow.get ()->postMessage (stateChangedMsg);
        }
        default:
            // Not a video message for the application
            break;
        }
    }

    void handleControlPannelMessage (const CMP::ControlPannelMessage& _message)
    {
        switch (_message.getType ())
        {
        case CMP::ControlPannelMessage::Type::Play:
        {
            CMP::VideoMessage* playMsg =
                new CMP::VideoMessage (CMP::VideoMessage::Type::Play);
            video->postMessage (playMsg);
            break;
        }
        case CMP::ControlPannelMessage::Type::Pause:
        {
            CMP::VideoMessage* pauseMsg =
                new CMP::VideoMessage (CMP::VideoMessage::Type::Pause);
            video->postMessage (pauseMsg);
            break;
        }
        case CMP::ControlPannelMessage::Type::Restart:
        {
            CMP::VideoMessage* restartMsg =
                new CMP::VideoMessage (CMP::VideoMessage::Type::Restart);
            video->postMessage (restartMsg);
            break;
        }
        default:
            break;
        }
    }

    //==============================================================================
    void initialise (const juce::String& /*commandLine*/) override
    {
        externalInfo = new CMP::ExternalInfo ();
        CMP::Video::setMainThread (this);
        CMP::MainComponent::setMainApplication (this);
        if (not externalInfo->setCSVPath ("Cues.csv"))
        {
            juce::AlertWindow::showMessageBoxAsync (
                juce::AlertWindow::AlertIconType::WarningIcon,
                "Error",
                "The CSV file does not exist",
                "OK",
                nullptr,
                juce::ModalCallbackFunction::create ([] (int) {
                    juce::MessageManager::getInstance ()->stopDispatchLoop ();
                }));
            return;
        }
        if (not externalInfo->setVideoPath ("Video.mp4"))
        {
            juce::AlertWindow::showMessageBoxAsync (
                juce::AlertWindow::AlertIconType::WarningIcon,
                "Error",
                "The video file does not exist",
                "OK",
                nullptr,
                juce::ModalCallbackFunction::create ([] (int) {
                    juce::MessageManager::getInstance ()->stopDispatchLoop ();
                }));
            return;
        }
        if (not externalInfo->setupCSV ())
        {
            juce::AlertWindow::showMessageBoxAsync (
                juce::AlertWindow::AlertIconType::WarningIcon,
                "Error",
                "The CSV file is not valid",
                "OK",
                nullptr,
                juce::ModalCallbackFunction::create ([] (int) {
                    juce::MessageManager::getInstance ()->stopDispatchLoop ();
                }));
            return;
        }
        controlPannelWindow.reset (new ControlPannelWindow (
            getApplicationName (), current_timecode, *externalInfo));

        video = new CMP::Video (externalInfo->getVideoFile ());
        timerUpdaterThread.startThread ();

        // Display cues
        CMP::ControlPannelMessage* refreshMsg = new CMP::ControlPannelMessage (
            CMP::ControlPannelMessage::Type::Refresh, "Cues");
        controlPannelWindow->postMessage (refreshMsg);
    }

    void shutdown () override
    {
        delete video;
        delete externalInfo;

        controlPannelWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit () override
    {
        // This is called when the app is being asked to quit: you can ignore
        // this request and let the app carry on running, or call quit() to
        // allow the app to close.
        quit ();
    }

    void anotherInstanceStarted (const juce::String& /*commandLine*/) override
    {
        // When another instance of the app is launched while this one is
        // running, this method is invoked, and the commandLine parameter tells
        // you what the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains the cues and the
       pause button.
    */
    class ControlPannelWindow : public juce::DocumentWindow,
                                public juce::MessageListener
    {
    public:
        ControlPannelWindow (juce::String name,
                             CMP::Timecode& _current_timecode,
                             CMP::ExternalInfo& _externalInfo)
            : DocumentWindow (
                  name,
                  juce::Desktop::getInstance ()
                      .getDefaultLookAndFeel ()
                      .findColour (juce::ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons),
              current_timecode (_current_timecode)
        {
            setUsingNativeTitleBar (true);
            content = new CMP::MainComponent (current_timecode, _externalInfo);
            setContentOwned (content, true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
#else
            setResizable (true, true);
            centreWithSize (getWidth (), getHeight ());
#endif

            setVisible (true);
        }

        void closeButtonPressed () override
        {
            // This is called when the user tries to close this window. Here,
            // we'll just ask the app to quit when this happens, but you can
            // change this to do whatever you need.
            JUCEApplication::getInstance ()->systemRequestedQuit ();
        }

        void handleMessage (const juce::Message& _message) override
        {
            content->handleMessage (_message);
        }

        /* Note: Be careful if you override any DocumentWindow methods - the
           base class uses a lot of them, so by overriding you might break its
           functionality. It's best to do all your work in your content
           component instead, but if you really have to override any
           DocumentWindow methods, make sure your subclass also calls the
           superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPannelWindow)

        CMP::MainComponent* content{nullptr};
        CMP::Timecode& current_timecode;
    };

    class TimerUpdaterThread : public juce::Thread
    {
    public:
        TimerUpdaterThread (CMPApplication& _app)
            : Thread ("TimerUpdaterThread"), app (_app)
        {
        }
        void run () override
        {
            while (!threadShouldExit ())
            {
                updateTimer ();
                juce::Thread::sleep (10);
            }
        }

        void updateTimer ()
        {
            if (app.video->isPlaying ())
            {
                auto nanosec = app.video->getRunningTime ();
                auto tc = CMP::Timecode (nanosec);
                if (tc != app.current_timecode)
                {
                    app.current_timecode.setFramesTotal (tc.getFramesTotal ());
                    CMP::ControlPannelMessage* timecodeMsg =
                        new CMP::ControlPannelMessage (
                            CMP::ControlPannelMessage::Type::Refresh, "Timer");
                    app.controlPannelWindow.get ()->postMessage (timecodeMsg);
                }
            }
        }

    private:
        CMPApplication& app;
    } timerUpdaterThread{*this};

private:
    std::unique_ptr<ControlPannelWindow> controlPannelWindow;
    CMP::Video* video{nullptr};

    CMP::Timecode current_timecode;

    CMP::ExternalInfo* externalInfo;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (CMPApplication)
