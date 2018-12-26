#include "betomnita/BetomnitaGame.hpp"

#include "app/Debug.hpp"
#include "betomnita/Cursor.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/state/GamePlayState.hpp"
#include "betomnita/state/MainMenuState.hpp"
#include "betomnita/state/PauseState.hpp"
#include "game/graphics/Text.hpp"
#include "game/state/StateMachine.hpp"

#include <random>
#include <sstream>

namespace Betomnita
{
    BetomnitaGame* BetomnitaGame::s_instance = nullptr;

    BetomnitaGame::BetomnitaGame() : m_stateMachine( std::make_unique< Game::StateMachine< Resources::StateId > >() ), m_cursor( std::make_unique< Cursor >() )
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
        m_stateMachine->RegisterState( std::make_shared< States::MainMenuState >() );
        m_stateMachine->RegisterState( std::make_shared< States::GamePlayState >() );
        m_stateMachine->RegisterState( std::make_shared< States::PauseState >() );
        m_stateMachine->PushState( Resources::StateId::MainMenu );

        Game::EventSystem::Event< Resources::EventId::OnStart >::Dispatch();
        GenericGame::OnStart();
    };

    void BetomnitaGame::OnUpdate( const sf::Time& dt )
    {
        const auto& mousePosition = GetMousePosition();
        m_cursor->SetPosition( mousePosition );
        m_stateMachine->OnUpdate( dt );
        Game::EventSystem::Event< Resources::EventId::OnUpdate >::Dispatch< const sf::Time& >( dt );
        GenericGame::OnUpdate( dt );
    }

    void BetomnitaGame::OnRender( sf::RenderTarget& target )
    {
        target.clear( Resources::Background );
        m_stateMachine->OnRender( target );
        m_cursor->Render( target );
        m_window.display();

        Game::EventSystem::Event< Resources::EventId::OnRender >::Dispatch< sf::RenderTarget& >( target );
        GenericGame::OnRender( target );
    }

    void BetomnitaGame::OnVideoSettingsChanged()
    {
        Game::EventSystem::Event< Resources::EventId::OnVideoSettingChanged >::Dispatch();
        GenericGame::OnVideoSettingsChanged();
    }

    void BetomnitaGame::OnClose()
    {
        Game::EventSystem::Event< Resources::EventId::OnClose >::Dispatch();
        GenericGame::OnClose();
    }

    void BetomnitaGame::OnKeyPressed( const sf::Event::KeyEvent& key )
    {
        switch( key.code )
        {
            case sf::Keyboard::Key::F12:
            {
                RequestScreenshot( Resources::ScreenshotPath );
            }
            break;
        }
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::Dispatch< const sf::Event::KeyEvent& >( key );
        GenericGame::OnKeyPressed( key );
    }

    void BetomnitaGame::OnKeyReleased( const sf::Event::KeyEvent& key )
    {
        Game::EventSystem::Event< Resources::EventId::OnKeyReleased >::Dispatch< const sf::Event::KeyEvent& >( key );
        GenericGame::OnKeyReleased( key );
    }

    void BetomnitaGame::OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button )
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::Dispatch< const sf::Vector2f&, sf::Mouse::Button >( position, button );
        GenericGame::OnMouseButtonPressed( position, button );
    }

    void BetomnitaGame::OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button )
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::Dispatch< const sf::Vector2f&, sf::Mouse::Button >( position, button );
        GenericGame::OnMouseButtonReleased( position, button );
    }

    void BetomnitaGame::OnMouseMoved( const sf::Vector2f& position )
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::Dispatch< const sf::Vector2f& >( position );
        GenericGame::OnMouseMoved( position );
    }
}