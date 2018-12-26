#pragma once
#include <SFML/Graphics.hpp>

namespace Game::Graphics
{
    class Polygon;
}

namespace Betomnita
{
    class Cursor
    {
    public:
        Cursor();
        ~Cursor();

        void Render( sf::RenderTarget& target );

        void SetPosition( const sf::Vector2f& value );
        const sf::Vector2f& GetPosition() const
        {
            return m_position;
        }

    private:
        sf::Vector2f m_position;
        std::unique_ptr< Game::Graphics::Polygon > m_shape;
    };
}