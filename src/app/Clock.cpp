#include "app/Clock.hpp"

namespace App
{
    App::Clock::Clock() {}

    sf::Time App::Clock::GetElapsedTime() const { return m_clock.getElapsedTime() - m_delay; }

    sf::Time App::Clock::Restart()
    {
        auto elapsed = m_clock.restart() - m_delay;
        m_delay = sf::Time::Zero;
        return elapsed;
    }

    sf::Time App::Clock::Pause()
    {
        m_delayClock.restart();
        return GetElapsedTime();
    }

    sf::Time App::Clock::Unpause()
    {
        m_delay += m_delayClock.getElapsedTime();
        return GetElapsedTime();
    }
}