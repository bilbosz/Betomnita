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
            m_top.SetPosition( m_position );
            m_bottom.SetPosition( m_position );
        }
        sf::Vector2f GetPosition() const
        {
            return m_position;
        }

    private:
        sf::Vector2f m_position;
        Graphics::Polygon m_bottom, m_top;
    };
}