#pragma once
#include "game/State.hpp"
#include "resource/Resource.hpp"

#include <memory>

namespace Game::Graphics
{
    class Text;
}

namespace Betomnita
{
    class GamePlayState final
        : public Game::State< Resource::StateId >
        , public std::enable_shared_from_this< GamePlayState >
    {
    public:
        GamePlayState();
        ~GamePlayState();

        void OnRegister() override;
        void OnUnregister() override;
        void OnActivate() override;
        void OnDeactivate() override;
        void OnForeground() override;
        void OnBackground() override;
        void OnUpdate( const sf::Time& dt ) override;
        void OnRender( sf::RenderTarget& target ) override;

        void OnMainMenuRequest();

    private:
        std::unique_ptr< Game::Graphics::Text > m_timerText;
        sf::Time m_timer;
    };
}