#pragma once
#include "game/graphics/Polygon.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

namespace Betomnita
{
    class Cursor
    {
    public:
        Cursor();
        ~Cursor() = default;

        void Render( sf::RenderTarget& target );

        void SetPosition( const sf::Vector2f& value );
        [[nodiscard]] const sf::Vector2f& GetPosition() const
        {
            return m_position;
        }

        void Hide();
        void Show();

    private:
        bool m_shown = true;
        sf::Vector2f m_position;
        std::vector< Game::Graphics::Polygon > m_shape;
    };
}