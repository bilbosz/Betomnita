#include "betomnita/state/MainMenuState.hpp"

#include "app/Application.hpp"
#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/layout/MainMenuLayout.hpp"
#include "game/StateMachine.hpp"

namespace Betomnita
{
    MainMenuState::MainMenuState() : State( Resource::StateId::MainMenu ), m_layout( std::make_unique< Layout::MainMenuLayout >( this ) )
    {
    }

    MainMenuState::~MainMenuState()
    {
    }

    void MainMenuState::OnRegister()
    {
        State::OnRegister();
    }

    void MainMenuState::OnUnregister()
    {
        State::OnUnregister();
    }

    void MainMenuState::OnActivate()
    {
        State::OnActivate();
    }

    void MainMenuState::OnDeactivate()
    {
        State::OnDeactivate();
    }

    void MainMenuState::OnForeground()
    {
        m_layout->Show();
        State::OnForeground();
    }

    void MainMenuState::OnBackground()
    {
        m_layout->Hide();
        State::OnBackground();
    }

    void MainMenuState::OnUpdate( const sf::Time& dt )
    {
        if( IsForeground() )
        {
            m_layout->OnUpdate( dt );
        }
        State::OnUpdate( dt );
    }

    void MainMenuState::OnRender( sf::RenderTarget& target )
    {
        if( IsForeground() )
        {
            m_layout->OnRender( target );
        }
        State::OnRender( target );
    }

    void MainMenuState::OnNewGameRequest()
    {
        Betomnita::BetomnitaGame::GetInstance()->GetStateMachine()->PushState( Resource::StateId::GamePlay );
    }

    void MainMenuState::OnExitRequest()
    {
        App::Application::GetInstance()->RequestShutdown();
    }
}