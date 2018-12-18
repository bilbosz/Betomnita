#pragma once
#include "betomnita/resources/Resources.hpp"
#include "game/State.hpp"

#include <memory>

namespace Game::Graphics
{
    class Text;
}

namespace Betomnita::States
{
    class GamePlayState final
        : public Game::State< Resources::StateId >
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