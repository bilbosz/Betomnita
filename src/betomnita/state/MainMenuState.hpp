#pragma once
#include "betomnita/resources/Resources.hpp"
#include "game/state/State.hpp"

namespace Betomnita::Layouts
{
    class MainMenuLayout;
}

namespace Betomnita::States
{
    class MainMenuState final
        : public Game::State< Resources::StateId >
        , public std::enable_shared_from_this< MainMenuState >
    {
    public:
        MainMenuState();
        ~MainMenuState();

        void OnRegister() override;
        void OnUnregister() override;
        void OnActivate() override;
        void OnDeactivate() override;
        void OnForeground() override;
        void OnBackground() override;
        void OnUpdate( const sf::Time& dt ) override;
        void OnRender( sf::RenderTarget& target ) override;
        void OnNewGameRequest();
        void OnExitRequest();

    private:
        std::unique_ptr< Layouts::MainMenuLayout > m_layout;
    };
}