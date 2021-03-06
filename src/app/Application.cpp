#include "app/Application.hpp"

#include "app/Debug.hpp"

namespace App
{
    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        ASSERT( !s_instance, L"There can be only one instance of the application" );
        s_instance = this;
#ifdef REDIRECT_CONSOLE_OUT
        VERIFY( m_stdout = freopen( "stdout.log", "w+", stdout ) );
        VERIFY( m_stderr = freopen( "stderr.log", "w+", stderr ) );
#endif
    }

    Application::~Application()
    {
        ASSERT( s_instance, L"Application should exist before calling destructor" );
        s_instance = nullptr;
#ifdef REDIRECT_CONSOLE_OUT
        VERIFY( fclose( m_stdout ) == 0 );
        VERIFY( fclose( m_stderr ) == 0 );
#endif
    }

    Application* Application::GetInstance()
    {
        ASSERT( s_instance, L"Application is not created yet" );
        return s_instance;
    }

    void Application::Run()
    {
        ApplyVideoSettings();
        m_clock.Restart();
        OnStart();
        while( !m_shutdownRequested )
        {
            if( m_videoSettingsApplyRequested )
            {
                sf::Time start = m_clock.Pause();
                ApplyVideoSettings();
                sf::Time finish = m_clock.Unpause();
            }
            Tick();
        }
        OnClose();
    }

    void Application::SetWindowTitle( const std::wstring& value )
    {
        m_windowTitle = value;
        RequestApplyVideoSettings();
    }

    void Application::SetVideoMode( const sf::VideoMode& value )
    {
        m_videoMode = value;
        RequestApplyVideoSettings();
    }

    void Application::SetFullscreen()
    {
        m_windowStyle = sf::Style::Fullscreen;
        RequestApplyVideoSettings();
    }

    void Application::SetBorderlessWindow()
    {
        m_windowStyle = sf::Style::None;
        RequestApplyVideoSettings();
    }

    void Application::SetBorderedWindow( bool resizable, bool closeButton )
    {
        m_windowStyle = sf::Style::Titlebar | resizable * sf::Style::Resize | closeButton * sf::Style::Close;
        RequestApplyVideoSettings();
    }

    bool Application::IsFullscreen()
    {
        return !!( m_windowStyle & sf::Style::Fullscreen );
    }

    void Application::ShowCursor()
    {
        m_cursorShown = true;
        RequestApplyVideoSettings();
    }

    void Application::HideCursor()
    {
        m_cursorShown = false;
        RequestApplyVideoSettings();
    }

    void Application::SetAntialiasingLevel( unsigned int value )
    {
        m_antialiasingLevel = value;
        RequestApplyVideoSettings();
    }

    void Application::EnableVerticalSync()
    {
        m_verticalSync = true;
    }

    void Application::DisableVerticalSync()
    {
        m_verticalSync = false;
    }

    void Application::GrabCursor()
    {
        m_cursorGrabbed = true;
        RequestApplyVideoSettings();
    }

    void Application::ReleaseCursor()
    {
        m_cursorGrabbed = false;
        RequestApplyVideoSettings();
    }

    void Application::ApplyVideoSettings()
    {
        ASSERT( !IsFullscreen() || m_videoMode.isValid(), L"Fullscreen mode is not valid" );
        m_videoSettingsApplyRequested = false;
        auto context = sf::ContextSettings();
        context.antialiasingLevel = m_antialiasingLevel;
        m_window.create( m_videoMode, m_windowTitle, m_windowStyle, context );
        m_window.setFramerateLimit( m_framerateLimit );
        m_window.setMouseCursorVisible( m_cursorShown );
        m_window.setMouseCursorGrabbed( m_cursorGrabbed );
        m_window.setVerticalSyncEnabled( m_verticalSync );
        m_window.setPosition( m_windowPosition );
        OnVideoSettingsChanged();
    }

    void Application::Tick()
    {
        auto now = m_clock.GetElapsedTime();
        auto diff = now - m_lastTickTime;
        HandleEvents();
        if( diff == sf::Time::Zero )
        {
            return;
        }
        OnUpdate( m_timeSkew * diff );
        OnRender( m_window );
        if( !m_screenshotRequest.empty() )
        {
            TakeScreenshot();
            m_screenshotRequest.clear();
        }
        m_lastTickTime = now;
    }

    void Application::HandleEvents()
    {
        sf::Event e;
        while( m_window.pollEvent( e ) )
        {
            OnEvent( e );
        }
    }

    void Application::TakeScreenshot()
    {
        sf::Vector2u windowSize = m_window.getSize();
        sf::Texture texture;
        texture.create( windowSize.x, windowSize.y );
        texture.update( m_window );
        texture.copyToImage().saveToFile( m_screenshotRequest );
        OnScreenshotTaken( m_screenshotRequest );
    }

    void Application::SetWindowPosition( const sf::Vector2i& value )
    {
        m_windowPosition = value;
        m_videoSettingsApplyRequested = true;
    }
}