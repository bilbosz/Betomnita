#pragma once
#include "app/Clock.hpp"
#include "resource/Resource.hpp"

#include <SFML/Graphics.hpp>

namespace App
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void RequestShutdown() { m_shutdownRequested = true; }
        void RequestApplyVideoSettings() { m_videoSettingsApplyRequested = true; }

        void SetWindowTitle( const std::wstring& value )
        {
            m_windowTitle = value;
            RequestApplyVideoSettings();
        }
        std::wstring GetWindowTitle() const { return m_windowTitle; }

        void SetVideoMode( const sf::VideoMode& value )
        {
            m_videoMode = value;
            RequestApplyVideoSettings();
        }
        sf::VideoMode GetVideoMode() const { return m_videoMode; }

        void SetFullscreen()
        {
            m_windowStyle = sf::Style::Fullscreen;
            RequestApplyVideoSettings();
        }
        void SetBorderlessWindow()
        {
            m_windowStyle = sf::Style::None;
            RequestApplyVideoSettings();
        }
        void SetBorderedWindow( bool resizable = true, bool closeButton = true )
        {
            m_windowStyle = sf::Style::Titlebar | resizable * sf::Style::Resize | closeButton * sf::Style::Close;
            RequestApplyVideoSettings();
        }
        bool IsFullscreen() { return !!( m_windowStyle & sf::Style::Fullscreen ); }

        void ShowCursor()
        {
            m_cursorShown = true;
            RequestApplyVideoSettings();
        }
        void HideCursor()
        {
            m_cursorShown = false;
            RequestApplyVideoSettings();
        };

        void SetFramerateLimit( unsigned int value ) { m_framerateLimit = value; }
        unsigned int GetFramerateLimit() const { return m_framerateLimit; }

        void SetAntialiasingLevel( unsigned int value )
        {
            m_antialiasingLevel = value;
            RequestApplyVideoSettings();
        }
        unsigned int GetAntialiasingLevel() { return m_antialiasingLevel; }

        void SetTimeSkew( float value ) { m_timeSkew = value; }
        float GetTimeSkew() const { return m_timeSkew; }

        void GrabCursor()
        {
            m_cursorGrabbed = true;
            RequestApplyVideoSettings();
        }
        void ReleaseCursor()
        {
            m_cursorGrabbed = false;
            RequestApplyVideoSettings();
        }
        bool IsCursorGrabbed() const { return m_cursorGrabbed; }

        const Clock& GetClock() const { return m_clock; }

    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate( sf::Time dt ) = 0;
        virtual void OnRender( sf::Time dt ) = 0;
        virtual void OnEvent( const sf::Event& e ) = 0;
        virtual void OnVideoSettingsChanged() = 0;
        virtual void OnClose() = 0;
        virtual void OnKeyPressed( const sf::Event::KeyEvent& key ) = 0;
        virtual void OnKeyReleased( const sf::Event::KeyEvent& key ) = 0;
        virtual void OnMouseButtonPressed( const sf::Event::MouseButtonEvent& mouseButton ) = 0;
        virtual void OnMouseButtonReleased( const sf::Event::MouseButtonEvent& mouseButton ) = 0;
        virtual void OnMouseMoved( const sf::Event::MouseMoveEvent& mouseMove ) = 0;

        sf::RenderWindow m_window;

    private:
        void Tick();
        void ApplyVideoMode();
        void HandleEvents();

        std::wstring m_windowTitle = Resource::WindowTitle;

        Clock m_clock;
        sf::VideoMode m_videoMode = sf::VideoMode::getDesktopMode();
        uint32_t m_windowStyle = sf::Style::Default;
        unsigned int m_antialiasingLevel = 0;
        unsigned int m_framerateLimit = 60;
        bool m_cursorShown = false;

        float m_timeSkew = 1.0f;

        bool m_cursorGrabbed = true;

        bool m_shutdownRequested = false;
        bool m_videoSettingsApplyRequested = false;

        sf::Time m_lastTickTime;
    };
}