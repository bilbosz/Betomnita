#include "betomnita/state/MainMenuState.hpp"

#include "app/Application.hpp"
#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/layout/MainMenuLayout.hpp"
#include "game/StateMachine.hpp"

namespace Betomnita::States
{
    MainMenuState::MainMenuState() : State( Resources::StateId::MainMenu ), m_layout( std::make_unique< Layouts::MainMenuLayout >( this ) )
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
        BetomnitaGame::GetInstance()->GetStateMachine()->PushState( Resources::StateId::GamePlay );
    }

    void MainMenuState::OnExitRequest()
    {
        App::Application::GetInstance()->RequestShutdown();
    }
}