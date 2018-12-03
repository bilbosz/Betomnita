#include "betomnita/BetomnitaGame.hpp"

#include "app/Debug.hpp"
#include "betomnita/Cursor.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/state/MainMenuState.hpp"
#include "game/StateMachine.hpp"
#include "game/graphics/Text.hpp"

#include <random>
#include <sstream>

namespace Betomnita
{
    BetomnitaGame* BetomnitaGame::s_instance = nullptr;

    BetomnitaGame::BetomnitaGame() : m_stateMachine( std::make_unique< Game::StateMachine< Resource::StateId > >() ), m_cursor( std::make_unique< Cursor >() )
    {
        ASSERT( !s_instance, L"There can be only one instance of Betomnita game" );
        s_instance = this;
    }

    BetomnitaGame::~BetomnitaGame()
    {
        ASSERT( s_instance, L"Betomnita game should exist before calling destructor" );
        s_instance = nullptr;
    }

    BetomnitaGame* BetomnitaGame::GetInstance()
    {
        ASSERT( s_instance, L"Betomnita game is not created yet" );
        return s_instance;
    }

    void BetomnitaGame::OnStart()
    {
        m_stateMachine->RegisterState( std::make_shared< MainMenuState >() );
        m_stateMachine->PushState( Resource::StateId::MainMenu );
        Event< Resource::EventId::OnStart >::Dispatch( std::make_tuple<>() );
        GenericGame::OnStart();
    };

    void BetomnitaGame::OnUpdate( const sf::Time& dt )
    {
        const auto& mousePosition = GetMousePosition();
        m_cursor->SetPosition( mousePosition );
        std::wostringstream out;
        out << L"(" << mousePosition.x << L", " << mousePosition.y << L")";
        m_stateMachine->OnUpdate( dt );
        Event< Resource::EventId::OnUpdate >::Dispatch( std::make_tuple< const sf::Time& >( dt ) );
        GenericGame::OnUpdate( dt );
    }

    void BetomnitaGame::OnRender( sf::RenderTarget& target )
    {
        target.clear( sf::Color( 200, 230, 255, 255 ) );
        m_stateMachine->OnRender( target );
        m_cursor->Render( target );
        m_window.display();

        // Event< Resource::EventId::OnRender >::Dispatch( std::make_tuple< sf::RenderTarget& >( target ) );
        GenericGame::OnRender( target );
    }

    void BetomnitaGame::OnVideoSettingsChanged()
    {
        Event< Resource::EventId::OnVideoSettingChanged >::Dispatch( std::make_tuple<>() );
        GenericGame::OnVideoSettingsChanged();
    }

    void BetomnitaGame::OnClose()
    {
        Event< Resource::EventId::OnClose >::Dispatch( std::make_tuple<>() );
        GenericGame::OnClose();
    }

    void BetomnitaGame::OnKeyPressed( const sf::Event::KeyEvent& key )
    {
        switch( key.code )
        {
            case sf::Keyboard::Key::Escape:
            {
                RequestShutdown();
            }
            break;
            case sf::Keyboard::Key::F12:
            {
                RequestScreenshot( Resource::ScreenshotPath );
            }
            break;
        }
        Event< Resource::EventId::OnKeyPressed >::Dispatch( std::make_tuple< const sf::Event::KeyEvent& >( key ) );
        GenericGame::OnKeyPressed( key );
    }

    void BetomnitaGame::OnKeyReleased( const sf::Event::KeyEvent& key )
    {
        Event< Resource::EventId::OnKeyReleased >::Dispatch( std::make_tuple< const sf::Event::KeyEvent& >( key ) );
        GenericGame::OnKeyReleased( key );
    }

    void BetomnitaGame::OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button )
    {
        switch( button )
        {
            case sf::Mouse::Button::Left:
                break;
            case sf::Mouse::Button::Right:
                break;
            case sf::Mouse::Button::Middle:
                break;
        }
        // Event< Resource::EventId::OnMouseButtonPressed >::Dispatch( std::make_tuple< const sf::Vector2f&, sf::Mouse::Button >( position, button ) );
        GenericGame::OnMouseButtonPressed( position, button );
    }

    void BetomnitaGame::OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button )
    {
        GenericGame::OnMouseButtonReleased( position, button );
    }

    void BetomnitaGame::OnMouseMoved( const sf::Vector2f& position )
    {
        GenericGame::OnMouseMoved( position );
        Event< Resource::EventId::OnMouseMoved >::Dispatch( std::make_tuple< const sf::Vector2f& >( position ) );
    }
}