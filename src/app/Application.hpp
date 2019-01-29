#pragma once
#include "app/Clock.hpp"
#include "project/Config.hpp"

#include <SFML/Graphics.hpp>
#include <cstdio>

namespace App
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        static Application* GetInstance();

        void Run();

        void RequestShutdown()
        {
            m_shutdownRequested = true;
        }
        void RequestApplyVideoSettings()
        {
            m_videoSettingsApplyRequested = true;
        }

        void SetWindowTitle( const std::wstring& value );
        std::wstring GetWindowTitle() const
        {
            return m_windowTitle;
        }

        void SetVideoMode( const sf::VideoMode& value );
        sf::VideoMode GetVideoMode() const
        {
            return m_videoMode;
        }

        void SetFullscreen();
        void SetBorderlessWindow();
        void SetBorderedWindow( bool resizable = true, bool closeButton = true );
        bool IsFullscreen();

        void ShowCursor();
        void HideCursor();

        void SetFramerateLimit( unsigned int value )
        {
            m_framerateLimit = value;
        }
        unsigned int GetFramerateLimit() const
        {
            return m_framerateLimit;
        }

        void SetAntialiasingLevel( unsigned int value );
        unsigned int GetAntialiasingLevel()
        {
            return m_antialiasingLevel;
        }

        void EnableVerticalSync();
        void DisableVerticalSync();
        bool GetVerticalSync() const
        {
            return m_verticalSync;
        }

        void SetTimeSkew( float value )
        {
            m_timeSkew = value;
        }
        float GetTimeSkew() const
        {
            return m_timeSkew;
        }

        void GrabCursor();
        void ReleaseCursor();
        bool IsCursorGrabbed() const
        {
            return m_cursorGrabbed;
        }

        sf::Time GetApplicationTime() const
        {
            return m_clock.GetElapsedTime();
        }

        void RequestScreenshot( const std::string& filename )
        {
            m_screenshotRequest = filename;
        }

    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate( const sf::Time& dt ) = 0;
        virtual void OnRender( sf::RenderTarget& target ) = 0;
        virtual void OnEvent( const sf::Event& e ) = 0;
        virtual void OnVideoSettingsChanged() = 0;
        virtual void OnClose() = 0;
        virtual void OnScreenshotTaken( const std::string& filename ) = 0;

        sf::RenderWindow m_window;

    private:
        void Tick();
        void ApplyVideoSettings();
        void HandleEvents();
        void TakeScreenshot();

        static Application* s_instance;

        std::wstring m_windowTitle;

        Clock m_clock;
        sf::VideoMode m_videoMode = sf::VideoMode::getDesktopMode();
        uint32_t m_windowStyle = sf::Style::Default;
        unsigned int m_antialiasingLevel = 0;
        unsigned int m_framerateLimit = 60;
        bool m_verticalSync = false;
        bool m_cursorShown = false;

        float m_timeSkew = 1.0f;

        bool m_cursorGrabbed = true;

        bool m_shutdownRequested = false;
        bool m_videoSettingsApplyRequested = false;
        std::string m_screenshotRequest;

        sf::Time m_lastTickTime;

#ifdef REDIRECT_CONSOLE_OUT
        FILE* m_stdout;
        FILE* m_stderr;
#endif
    };
}