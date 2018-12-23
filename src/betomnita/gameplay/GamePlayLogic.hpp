#pragma once
#include <SFML/Graphics.hpp>

namespace Betomnita::GamePlay
{
    class World;
    class View;
    class Cursor;

    class GamePlayLogic final
    {
    public:
        GamePlayLogic();
        ~GamePlayLogic();

        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );
        
    private:
        sf::Time m_time;
        //std::unique_ptr< World > m_world;
        //std::unique_ptr< View > m_view;
        //std::unique_ptr< Cursor > m_cursor;
    };
}