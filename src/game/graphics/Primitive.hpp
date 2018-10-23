#pragma once
#include "game/utils/Property.hpp"

#include <SFML/Graphics.hpp>

namespace Utils
{
    template< class Type >
    class Property;
}

namespace Graphics
{
    class Primitive
    {
    public:
        Primitive();
        virtual ~Primitive();

        virtual void Render( sf::RenderTarget& target );

        Utils::Property< sf::Vector2f > Position;
        Utils::Property< sf::Vector2f > Size;

        void SetHightlight( bool value )
        {
            m_highlight = value;
        }
        bool GetHightlight() const
        {
            return m_highlight;
        }

    protected:
        virtual void Init();

        virtual void OnPositionChange( const sf::Vector2f& newPosition );
        virtual void OnSizeChange( const sf::Vector2f& newSize );

    private:
        bool m_highlight = false;
        sf::RectangleShape m_debugRect;
    };
}