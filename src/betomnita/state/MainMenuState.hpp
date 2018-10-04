#pragma once
#include "game/State.hpp"

namespace Betomnita
{
    class MainMenuState
        : public Game::State
        , public std::enable_shared_from_this< MainMenuState >
    {
    public:
        MainMenuState();
        ~MainMenuState();

        void OnEnter() override;
        void OnUpdate( sf::Time dt ) override;
        void OnExit() override;
    };
}