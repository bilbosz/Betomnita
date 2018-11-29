#include "betomnita/state/MainMenuState.hpp"

#include "betomnita/layout/MainMenuLayout.hpp"
#include "app/Debug.hpp"

namespace Betomnita
{
    MainMenuState::MainMenuState() : State( Resource::StateId::MainMenu ), m_layout( std::make_unique< Layout::MainMenuLayout >() )
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
        m_layout->Show();
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
        m_layout->OnUpdate( dt );
        State::OnUpdate( dt );
    }

    void MainMenuState::OnRender( sf::RenderTarget& target )
    {
        m_layout->OnRender( target );
        State::OnRender( target );
    }
}