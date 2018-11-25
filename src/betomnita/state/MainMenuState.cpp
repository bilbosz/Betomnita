#include "betomnita/layout/MainMenuLayout.hpp"
#include "betomnita/state/MainMenuState.hpp"

#include "app/Debug.hpp"

namespace Betomnita
{
    MainMenuState::MainMenuState() : State( Resource::StateId::MainMenu )
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
        State::OnForeground();
    }

    void MainMenuState::OnBackground()
    {
        State::OnBackground();
    }

    void MainMenuState::OnUpdate( const sf::Time& dt )
    {
        State::OnUpdate( dt );
    }
}