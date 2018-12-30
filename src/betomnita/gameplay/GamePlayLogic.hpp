#pragma once
#include <SFML/Graphics.hpp>

namespace Game::Graphics
{
    class Polygon;
}

namespace Betomnita::GamePlay
{
    class World;
    class View;
    class Cursor;
    class Unit;

    class GamePlayLogic final
    {
    public:
        GamePlayLogic();
        ~GamePlayLogic();

        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );

    private:
        sf::Time m_time;
        std::unique_ptr< World > m_world; // mainly physics and world design
        //std::vector< std::unique_ptr< Unit > > m_units; // for executing units commands
    };
}