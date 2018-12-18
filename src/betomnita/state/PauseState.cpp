#include "betomnita/state/PauseState.hpp"

#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/layout/PauseLayout.hpp"
#include "betomnita/state/GamePlayState.hpp"
#include "game/StateMachine.hpp"

namespace Betomnita::States
{
    PauseState::PauseState() : State( Resources::StateId::Pause ), m_layout( std::make_unique< Layouts::PauseLayout >( this ) )
    {
    }

    PauseState::~PauseState()
    {
    }

    void PauseState::OnRegister()
    {
        State::OnRegister();
    }

    void PauseState::OnUnregister()
    {
        State::OnUnregister();
    }

    void PauseState::OnActivate()
    {
        State::OnActivate();
    }

    void PauseState::OnDeactivate()
    {
        State::OnDeactivate();
    }

    void PauseState::OnForeground()
    {
        m_layout->Show();
        State::OnForeground();
    }

    void PauseState::OnBackground()
    {
        m_layout->Hide();
        State::OnBackground();
    }

    void PauseState::OnUpdate( const sf::Time& dt )
    {
        if( IsForeground() )
        {
            m_layout->OnUpdate( dt );
        }
    }

    void PauseState::OnRender( sf::RenderTarget& target )
    {
        if( IsForeground() )
        {
            m_layout->OnRender( target );
        }
    }

    void PauseState::OnResumeRequest()
    {
        BetomnitaGame::GetInstance()->GetStateMachine()->PopState( GetId() );
    }

    void PauseState::OnMainMenuRequest()
    {
        auto sm = BetomnitaGame::GetInstance()->GetStateMachine();
        sm->PopState( GetId() );
#ifdef DEBUG
        auto gamePlay = std::dynamic_pointer_cast< States::GamePlayState >( sm->GetState( Resources::StateId::GamePlay ) );
        CHECK( gamePlay );
#else
        auto gamePlay = std::static_pointer_cast< States::GamePlayState >( sm->GetState( Resources::StateId::GamePlay ) );
#endif
        gamePlay->OnMainMenuRequest();
    }

    void PauseState::OnExitGameRequest()
    {
        App::Application::GetInstance()->RequestShutdown();
    }
}