#include "betomnita/state/GamePlayState.hpp"

#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/gameplay/GamePlayLogic.hpp"
#include "game/state/StateMachine.hpp"

namespace Betomnita::States
{
    GamePlayState::GamePlayState() : State( Resources::StateId::GamePlay )
    {
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::AddListener(
            { Resources::ListenerId::PauseRequest, false, [this]( const sf::Event::KeyEvent& key ) {
                 switch( key.code )
                 {
                     case sf::Keyboard::Key::Escape:
                         BetomnitaGame::GetInstance()->GetStateMachine()->PushState( Resources::StateId::Pause );
                         break;
                 }
             } } );
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
        m_logic = std::make_unique< Betomnita::GamePlay::GamePlayLogic >();
        State::OnActivate();
    }

    void GamePlayState::OnDeactivate()
    {
        m_logic.reset();
        State::OnDeactivate();
    }

    void GamePlayState::OnForeground()
    {
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::GetListener( Resources::ListenerId::PauseRequest ).IsEnabled = true;
        State::OnForeground();
    }

    void GamePlayState::OnBackground()
    {
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::GetListener( Resources::ListenerId::PauseRequest ).IsEnabled = false;
        State::OnBackground();
    }

    void GamePlayState::OnUpdate( const sf::Time& dt )
    {
        if( IsForeground() )
        {
            m_logic->Update( dt );
            State::OnUpdate( dt );
        }
    }

    void GamePlayState::OnRender( sf::RenderTarget& target )
    {
        target.clear();
        m_logic->Render( target );
        State::OnRender( target );
    }

    void GamePlayState::OnMainMenuRequest()
    {
        Betomnita::BetomnitaGame::GetInstance()->GetStateMachine()->PopState( GetId() );
    }
}