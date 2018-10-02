#include "app/Clock.hpp"

namespace App
{
    Clock::Clock() {}

    sf::Time Clock::GetElapsedTime() const { return m_clock.getElapsedTime() - m_delay; }

    sf::Time Clock::Restart()
    {
        auto elapsed = m_clock.restart() - m_delay;
        m_delay = sf::Time::Zero;
        return elapsed;
    }

    sf::Time Clock::Pause()
    {
        m_delayClock.restart();
        return GetElapsedTime();
    }

    sf::Time Clock::Unpause()
    {
        m_delay += m_delayClock.getElapsedTime();
        return GetElapsedTime();
    }
}