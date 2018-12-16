#include "betomnita/state/GamePlayState.hpp"

#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "game/StateMachine.hpp"
#include "game/graphics/Text.hpp"

#include <iomanip>
#include <sstream>

namespace Betomnita
{
    GamePlayState::GamePlayState() : State( Resource::StateId::GamePlay ), m_timerText( std::make_unique< Game::Graphics::Text >() )
    {
        Game::EventSystem::Event< Resource::EventId::OnKeyPressed >::AddListener( { Resource::ListenerId::PauseRequest, false, [this]( const sf::Event::KeyEvent& key ) {
                                                                                       switch( key.code )
                                                                                       {
                                                                                           case sf::Keyboard::Key::Escape:
                                                                                               Betomnita::BetomnitaGame::GetInstance()->GetStateMachine()->PushState(
                                                                                                   Resource::StateId::Pause );
                                                                                               break;
                                                                                       }
                                                                                   } } );
        const auto& aabb = Betomnita::BetomnitaGame::GetInstance()->GetModelAABB();
        m_timerText->SetColor( sf::Color::White );
        m_timerText->SetFont( *Resource::DebugFont );
        m_timerText->SetLineHeight( 0.025f );
        m_timerText->SetPosition( { aabb.left, aabb.top } );
    }

    GamePlayState::~GamePlayState()
    {
    }

    void GamePlayState::OnRegister()
    {
        State::OnRegister();
    }

    void GamePlayState::OnUnregister()
    {
        State::OnUnregister();
    }

    void GamePlayState::OnActivate()
    {
        m_timer = sf::Time::Zero;
        State::OnActivate();
    }

    void GamePlayState::OnDeactivate()
    {
        State::OnDeactivate();
    }

    void GamePlayState::OnForeground()
    {
        Game::EventSystem::Event< Resource::EventId::OnKeyPressed >::GetListener( Resource::ListenerId::PauseRequest ).IsEnabled = true;
        State::OnForeground();
    }

    void GamePlayState::OnBackground()
    {
        Game::EventSystem::Event< Resource::EventId::OnKeyPressed >::GetListener( Resource::ListenerId::PauseRequest ).IsEnabled = false;
        State::OnBackground();
    }

    void GamePlayState::OnUpdate( const sf::Time& dt )
    {
        if( IsForeground() )
        {
            m_timer += dt;
            std::wostringstream out;
            out << L"Time: " << std::setprecision( 4 ) << m_timer.asSeconds();
            m_timerText->SetString( out.str() );
            State::OnUpdate( dt );
        }
    }

    void GamePlayState::OnRender( sf::RenderTarget& target )
    {
        m_timerText->Render( target );
        State::OnRender( target );
    }

    void GamePlayState::OnMainMenuRequest()
    {
        Betomnita::BetomnitaGame::GetInstance()->GetStateMachine()->PopState( GetId() );
    }
}