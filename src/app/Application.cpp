#include "app/Application.hpp"

#include "app/Error.hpp"
#include "project/Config.hpp"

namespace App
{
    App::Application::Application()
    {
        VERIFY( m_stdout = freopen( "stdout.log", "w+", stdout ) );
        VERIFY( m_stderr = freopen( "stderr.log", "w+", stderr ) );
    }

    App::Application::~Application()
    {
        VERIFY( fclose( m_stdout ) == 0 );
        VERIFY( fclose( m_stderr ) == 0 );
    }

    void App::Application::Run()
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

    void App::Application::ApplyVideoMode()
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

    void App::Application::Tick()
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

    void App::Application::HandleEvents()
    {
        sf::Event e;
        while( m_window.pollEvent( e ) )
        {
            OnEvent( e );
            switch( e.type )
            {
                case sf::Event::Closed:
                {
                    m_shutdownRequested = true;
                }
                break;
                case sf::Event::KeyPressed:
                {
                    OnKeyPressed( e.key );
                }
                break;
                case sf::Event::KeyReleased:
                {
                    OnKeyPressed( e.key );
                }
                break;
                case sf::Event::MouseButtonPressed:
                {
                    OnMouseButtonPressed( e.mouseButton );
                }
                break;
                case sf::Event::MouseButtonReleased:
                {
                    OnMouseButtonReleased( e.mouseButton );
                }
                break;
                case sf::Event::MouseMoved:
                {
                    OnMouseMoved( e.mouseMove );
                }
                break;
            }
        }
    }
}