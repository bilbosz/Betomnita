#include "betomnita/state/PauseState.hpp"

#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/layout/PauseLayout.hpp"
#include "betomnita/state/GamePlayState.hpp"
#include "game/StateMachine.hpp"

namespace Betomnita
{
    PauseState::PauseState() : State( Resource::StateId::Pause ), m_layout( std::make_unique< Layout::PauseLayout >( this ) )
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
        Betomnita::BetomnitaGame::GetInstance()->GetStateMachine()->PopState( GetId() );
    }

    void PauseState::OnMainMenuRequest()
    {
        auto sm = Betomnita::BetomnitaGame::GetInstance()->GetStateMachine();
        sm->PopState( GetId() );
#ifdef DEBUG
        auto gamePlay = std::dynamic_pointer_cast< Betomnita::GamePlayState >( sm->GetState( Resource::StateId::GamePlay ) );
        CHECK( gamePlay );
#else
        auto gamePlay = std::static_pointer_cast< Betomnita::GamePlayState >( sm->GetState( Resource::StateId::GamePlay ) );
#endif
        gamePlay->OnMainMenuRequest();
    }

    void PauseState::OnExitGameRequest()
    {
        App::Application::GetInstance()->RequestShutdown();
    }
}