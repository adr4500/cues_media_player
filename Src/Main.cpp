/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
#include "ExternalInfo.h"
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
        if (auto* messagePtr =
                dynamic_cast<const CMP::VideoMessage*> (&_message))
        {
            handleVideoMessage (*messagePtr);
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
            break;
        default:
            // Not a video message for the application
            break;
        }
    }

    //==============================================================================
    void initialise (const juce::String& /*commandLine*/) override
    {
        CMP::Video::setMainThread (this);
        if (not externalInfo.setCSVPath ("Cues.csv"))
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
        if (not externalInfo.setVideoPath ("Video.mp4"))
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
        if (not externalInfo.setupCSV ())
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
        controlPannelWindow.reset (
            new ControlPannelWindow (getApplicationName ()));

        video = new CMP::Video (externalInfo.getVideoFile ());
    }

    void shutdown () override
    {
        delete video;

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


private:
    std::unique_ptr<ControlPannelWindow> controlPannelWindow;
    CMP::Video* video{nullptr};

    CMP::ExternalInfo externalInfo;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (CMPApplication)
