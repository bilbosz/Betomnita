#pragma once
#include "game/graphics/Polygon.hpp"

#include <SFML/Graphics.hpp>

namespace Betomnita
{
    class Cursor
    {
    public:
        Cursor();
        ~Cursor();

        void Render( sf::RenderTarget& target );

        void SetPosition( const sf::Vector2f& value )
        {
            m_position = value;
            m_shape.SetPosition( m_position );
        }
        const sf::Vector2f& GetPosition() const
        {
            return m_position;
        }

    private:
        sf::Vector2f m_position;
        Graphics::Polygon m_shape;
    };
}