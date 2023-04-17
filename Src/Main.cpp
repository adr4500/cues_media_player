/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
#include "ExternalInfo.h"
#include <cassert>

//==============================================================================
class CMPApplication : public juce::JUCEApplication
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

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        if (not externalInfo.setCSVPath ("Cues.csv"))
        {
            juce::AlertWindow::showMessageBoxAsync (
                juce::AlertWindow::AlertIconType::WarningIcon,
                "Error",
                "The CSV file does not exist",
                "OK",
                nullptr,
                juce::ModalCallbackFunction::create ([] (int) { juce::MessageManager::getInstance()->stopDispatchLoop(); }));
            return;
        }
        if (not externalInfo.setVideoPath ("Video.avi"))
        {
            juce::AlertWindow::showMessageBoxAsync (
                juce::AlertWindow::AlertIconType::WarningIcon,
                "Error",
                "The video file does not exist",
                "OK",
                nullptr,
                juce::ModalCallbackFunction::create ([] (int) { juce::MessageManager::getInstance()->stopDispatchLoop(); }));
            return;
        }
        if (not externalInfo.setupCSV ())
        {
            juce::AlertWindow::showMessageBoxAsync (
                juce::AlertWindow::AlertIconType::WarningIcon,
                "Error",
                "The CSV file is not valid",
                "OK",
                nullptr,
                juce::ModalCallbackFunction::create ([] (int) { juce::MessageManager::getInstance()->stopDispatchLoop(); }));
            return;
        }
        videoWindow.reset (new VideoWindow (getApplicationName ()));
        videoWindow.get()->loadVideo(externalInfo.getVideoFile());
        controlPannelWindow.reset (new ControlPannelWindow (getApplicationName ()));
    }

    void shutdown () override
    {
        // Add your application's shutdown code here..

        controlPannelWindow = nullptr; // (deletes our window)
        videoWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit () override
    {
        // This is called when the app is being asked to quit: you can ignore
        // this request and let the app carry on running, or call quit() to
        // allow the app to close.
        quit ();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is
        // running, this method is invoked, and the commandLine parameter tells
        // you what the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains the cues and the pause button.
    */
    class ControlPannelWindow : public juce::DocumentWindow
    {
    public:
        ControlPannelWindow (juce::String name)
            : DocumentWindow (
                  name,
                  juce::Desktop::getInstance ()
                      .getDefaultLookAndFeel ()
                      .findColour (juce::ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent (), true);

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

        /* Note: Be careful if you override any DocumentWindow methods - the
           base class uses a lot of them, so by overriding you might break its
           functionality. It's best to do all your work in your content
           component instead, but if you really have to override any
           DocumentWindow methods, make sure your subclass also calls the
           superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPannelWindow)
    };

    /*
        This class implements the fullscreen video window.
    */
    class VideoWindow : public juce::DocumentWindow
    {
    public:
        VideoWindow (juce::String name)
            : DocumentWindow (
                  name,
                  juce::Desktop::getInstance ()
                      .getDefaultLookAndFeel ()
                      .findColour (juce::ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new juce::VideoComponent (true), true);
            setFullScreen (false);
            setVisible (true);
        }

        void closeButtonPressed () override
        {
            // This is called when the user tries to close this window. Here,
            // we'll just ask the app to quit when this happens, but you can
            // change this to do whatever you need.
            JUCEApplication::getInstance ()->systemRequestedQuit ();
        }

        void loadVideo (juce::File videoFile)
        {
            juce::VideoComponent* videoComponent = dynamic_cast<juce::VideoComponent*> (getContentComponent ());
            assert(videoComponent != nullptr);
            if (videoComponent == nullptr)
            {
                juce::AlertWindow::showMessageBoxAsync (
                    juce::AlertWindow::AlertIconType::WarningIcon,
                    "Fatal Error",
                    "The video component could not be found",
                    "OK",
                    nullptr,
                    juce::ModalCallbackFunction::create ([] (int) { juce::MessageManager::getInstance ()->stopDispatchLoop (); }));
                return;
            }

            auto result = videoComponent->load (videoFile);
            if(result.failed())
            {
                juce::AlertWindow::showMessageBoxAsync (
                    juce::AlertWindow::AlertIconType::WarningIcon,
                    "Error",
                    "The video could not be loaded : " + result.getErrorMessage(),
                    "OK",
                    nullptr,
                    juce::ModalCallbackFunction::create ([] (int) { juce::MessageManager::getInstance()->stopDispatchLoop(); }));
            }
            else
            {
                videoComponent->play();
            }
        }

        /* Note: Be careful if you override any DocumentWindow methods - the
           base class uses a lot of them, so by overriding you might break its
           functionality. It's best to do all your work in your content
           component instead, but if you really have to override any
           DocumentWindow methods, make sure your subclass also calls the
           superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VideoWindow)
    };

private:
    std::unique_ptr<ControlPannelWindow> controlPannelWindow;
    std::unique_ptr<VideoWindow> videoWindow;
    CMP::ExternalInfo externalInfo;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (CMPApplication)
