#pragma once
#include "game/State.hpp"
#include "resource/Resource.hpp"

namespace Betomnita
{
    namespace Layout
    {
        class MainMenuLayout;
    }

    class MainMenuState final
        : public Game::State< Resource::StateId >
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

    private:
        std::unique_ptr< Layout::MainMenuLayout > m_layout;
    };
}