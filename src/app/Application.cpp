#include "app/Application.hpp"

#include "app/Error.hpp"
#include "project/Config.hpp"

namespace App
{
    Application::Application()
    {
#ifdef REDIRECT_CONSOLE_OUT
        VERIFY( m_stdout = freopen( "stdout.log", "w+", stdout ) );
        VERIFY( m_stderr = freopen( "stderr.log", "w+", stderr ) );
#endif
    }

    Application::~Application()
    {
#ifdef REDIRECT_CONSOLE_OUT
        VERIFY( fclose( m_stdout ) == 0 );
        VERIFY( fclose( m_stderr ) == 0 );
#endif
    }

    void Application::Run()
    {
        ApplyVideoMode();
        m_clock.Restart();
        OnStart();
        while( !m_shutdownRequested )
        {
            if( m_videoSettingsApplyRequested )
            {
                sf::Time start = m_clock.Pause();
                ApplyVideoMode();
                sf::Time finish = m_clock.Unpause();
            }
            Tick();
        }
        OnClose();
    }

    void Application::ApplyVideoMode()
    {
        ASSERT( m_videoMode.isValid(), L"Mode is not valid." );
        m_videoSettingsApplyRequested = false;
        auto context = sf::ContextSettings();
        context.antialiasingLevel = m_antialiasingLevel;
        m_window.create( m_videoMode, m_windowTitle, m_windowStyle, context );
        m_window.setFramerateLimit( m_framerateLimit );
        m_window.setMouseCursorVisible( m_cursorShown );
        m_window.setMouseCursorGrabbed( m_cursorGrabbed );
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
        OnRender( m_timeSkew * diff );
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
}