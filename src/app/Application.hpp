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

        void Run();

        void RequestShutdown() { m_shutdownRequested = true; }
        void RequestApplyVideoSettings() { m_videoSettingsApplyRequested = true; }

        void SetWindowTitle( std::wstring value ) { m_windowTitle = value; }
        std::wstring GetWindowTitle() const { return m_windowTitle; }

        void SetVideoMode( sf::VideoMode value ) { m_videoMode = value; }
        sf::VideoMode GetVideoMode() const { return m_videoMode; }

        void SetFullscreen() { m_windowStyle = sf::Style::Fullscreen; }
        void SetBorderlessWindow() { m_windowStyle = sf::Style::None; }
        void SetBorderedWindow( bool resizable = true, bool closeButton = true )
        {
            m_windowStyle = sf::Style::Titlebar | resizable * sf::Style::Resize | closeButton * sf::Style::Close;
        }
        bool IsFullscreen() { return !!( m_windowStyle & sf::Style::Fullscreen ); }

        void ShowCursor() { m_cursorShown = true; }
        void HideCursor() { m_cursorShown = false; };

        void SetFramerateLimit( unsigned int value ) { m_framerateLimit = value; }
        unsigned int GetFramerateLimit() const { return m_framerateLimit; }

        void SetAntialiasingLevel( unsigned int value ) { m_antialiasingLevel = value; }
        unsigned int GetAntialiasingLevel() { return m_antialiasingLevel; }

        void SetTimeSkew( float value ) { m_timeSkew = value; }
        float GetTimeSkew() const { return m_timeSkew; }

        const Clock& GetClock() const { return m_clock; }

    protected:
        sf::RenderWindow m_window;
        Clock m_clock;

    private:
        void Tick();
        void ApplyVideoMode();
        void HandleEvents();

        std::wstring m_windowTitle = Resource::WindowTitle;

        sf::VideoMode m_videoMode = sf::VideoMode::getDesktopMode();
        uint32_t m_windowStyle = sf::Style::Default;
        unsigned int m_antialiasingLevel = 0;
        unsigned int m_framerateLimit = 60;
        bool m_cursorShown = false;

        float m_timeSkew = 1.0f;

        bool m_shutdownRequested = false;
        bool m_videoSettingsApplyRequested = false;

        sf::Time m_lastTickTime;
    };
}