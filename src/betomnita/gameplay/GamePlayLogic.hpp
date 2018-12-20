#pragma once
#include <SFML/Graphics.hpp>

namespace Betomnita::GamePlay
{
    class GamePlayLogic final
    {
    public:
        GamePlayLogic();
        ~GamePlayLogic();

        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );

    private:
        sf::Time m_time;
    };
}