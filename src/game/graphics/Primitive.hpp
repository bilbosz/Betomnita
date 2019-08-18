#pragma once
#include <SFML/Graphics.hpp>

namespace Utils
{
    template< class Type >
    class Property;
}

namespace Game::Graphics
{
    class Primitive
    {
    public:
        Primitive();
        virtual ~Primitive();

        virtual void Render( sf::RenderTarget& target, const sf::Transform& transform );

        void SetPosition( const sf::Vector2f& value )
        {
            m_position = value;
            OnPositionChange();
        }
        [[nodiscard]] const sf::Vector2f& GetPosition() const
        {
            return m_position;
        }

        void SetSize( const sf::Vector2f& value )
        {
            m_size = value;
            OnSizeChange();
        }
        [[nodiscard]] const sf::Vector2f& GetSize() const
        {
            return m_size;
        }

        void SetHightlight( bool value )
        {
#ifdef DEBUG
            m_highlight = value;
#endif
        }

#ifndef DEBUG
        [[noreturn]]
#endif
        bool GetHightlight() const
        {
#ifdef DEBUG
            return m_highlight;
#endif
        }

        sf::Transform& Transform()
        {
            return m_transform;
        }

        void SetPivot( const sf::Vector2f& value )
        {
            m_pivot = value;
            OnPositionChange();
        }

        [[nodiscard]] const sf::Vector2f& GetPivot() const
        {
            return m_pivot;
        }

    protected:
        virtual void Init();

        virtual void OnPositionChange();
        virtual void OnTransformChange();
        virtual void OnPivotChange();
        virtual void OnSizeChange();

        sf::Vector2f m_position;
        sf::Transform m_transform;
        sf::Vector2f m_pivot;
        sf::Vector2f m_size;

    private:
#ifdef DEBUG
        bool m_highlight = false;
        sf::RectangleShape m_debugRect;
#endif
    };
}