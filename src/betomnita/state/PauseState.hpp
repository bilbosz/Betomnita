#pragma once
#include "game/State.hpp"
#include "resource/Resource.hpp"

namespace Betomnita
{
    namespace Layout
    {
        class PauseLayout;
    }

    class PauseState final
        : public Game::State< Resource::StateId >
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
        std::unique_ptr< Layout::PauseLayout > m_layout;
    };
}