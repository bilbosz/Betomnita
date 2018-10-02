#pragma once
#include "game/GenericGame.hpp"
#include "game/StateMachine.hpp"

namespace Betomnita
{
    class BetomnitaGame : public Game::GenericGame
    {
    public:
        enum class States
        {
            Undefined = -1,
            MainMenu,
            Gameplay,
            Pause,
        };

        BetomnitaGame();
        ~BetomnitaGame();

        void OnUpdate( sf::Time dt ) override;

    private:
        Game::StateMachine m_flowController;
    };
}