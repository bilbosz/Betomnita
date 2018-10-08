#pragma once
#include <SFML/Graphics.hpp>

namespace Betomnita
{
    class Cursor
    {
    public:
        Cursor();
        ~Cursor();
        void OnRender( sf::RenderTarget& target, const sf::Transform& transformation );

        void SetPosition( sf::Vector2f value )
        {
            m_position = value;
        }
        sf::Vector2f GetPosition() const
        {
            return m_position;
        }

    private:
        sf::Vector2f m_position;
        sf::Sprite m_asset;
    };
}