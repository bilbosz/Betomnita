#pragma once
#include <SFML/System/Clock.hpp>

namespace App
{
    class Clock
    {
    public:
        Clock();
        sf::Time GetElapsedTime() const;
        sf::Time Restart();
        sf::Time Pause();
        sf::Time Unpause();

    private:
        sf::Clock m_clock;
        sf::Clock m_delayClock;
        sf::Time m_delay;
    };
}