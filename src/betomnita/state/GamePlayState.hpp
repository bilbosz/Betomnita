#pragma once
#include "game/state/State.hpp"

#include <memory>

namespace Betomnita::Resources
{
    enum class StateId;
}

namespace Betomnita::GamePlay
{
    class GamePlayLogic;
    class Prototype;
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

        void ReloadPrototypes();
        Prototype GetPrototype( const std::wstring& name ) const;

        void OnMainMenuRequest();

    private:
        std::unique_ptr< Betomnita::GamePlay::GamePlayLogic > m_logic;
    };
}