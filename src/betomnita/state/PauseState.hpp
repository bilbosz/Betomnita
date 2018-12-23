#pragma once
#include "betomnita/resources/Resources.hpp"
#include "game/state/State.hpp"

namespace Betomnita::Layouts
{
    class PauseLayout;
}

namespace Betomnita::States
{
    class PauseState final
        : public Game::State< Resources::StateId >
        , public std::enable_shared_from_this< PauseState >
    {
    public:
        PauseState();
        ~PauseState();

        void OnRegister() override;
        void OnUnregister() override;
        void OnActivate() override;
        void OnDeactivate() override;
        void OnForeground() override;
        void OnBackground() override;
        void OnUpdate( const sf::Time& dt ) override;
        void OnRender( sf::RenderTarget& target ) override;

        void OnResumeRequest();
        void OnMainMenuRequest();
        void OnExitGameRequest();

    private:
        std::unique_ptr< Layouts::PauseLayout > m_layout;
    };
}