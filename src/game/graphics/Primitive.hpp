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

        void SetPosition( const sf::Vector2f value )
        {
            m_position = value;
            OnPositionChange( m_position );
        }
        const sf::Vector2f& GetPosition() const
        {
            return m_position;
        }

        void SetSize( const sf::Vector2f value )
        {
            m_size = value;
            OnSizeChange( m_size );
        }
        const sf::Vector2f& GetSize() const
        {
            return m_size;
        }

        void SetHightlight( bool value )
        {
#ifdef DEBUG
            m_highlight = value;
#endif
        }
        bool GetHightlight() const
        {
#ifdef DEBUG
			return m_highlight;
#endif
        }

    protected:
        virtual void Init();

        virtual void OnPositionChange( const sf::Vector2f& newPosition );
        virtual void OnSizeChange( const sf::Vector2f& newSize );

        sf::Vector2f m_position;
        sf::Vector2f m_size;

    private:
#ifdef DEBUG
        bool m_highlight = false;
        sf::RectangleShape m_debugRect;
#endif
    };
}