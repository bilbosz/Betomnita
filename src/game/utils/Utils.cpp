#include <game/utils/Utils.hpp>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

namespace Game::Utils
{
    template<>
    sf::Vector2f Cast( const b2Vec2& value )
    {
        return sf::Vector2f{ value.x, value.y };
    }

    template<>
    b2Vec2 Cast( const sf::Vector2f& value )
    {
        return b2Vec2( value.x, value.y );
    }
}