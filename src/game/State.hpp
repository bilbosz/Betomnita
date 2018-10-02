#pragma once
#include <SFML/System.hpp>

namespace Game
{
    class State
    {
    public:
        friend class StateMachine;
        State();
        virtual ~State();

    protected:
        virtual void OnEnter() = 0;
        virtual void OnUpdate( sf::Time dt ) = 0;
        virtual void OnExit() = 0;
    };
}