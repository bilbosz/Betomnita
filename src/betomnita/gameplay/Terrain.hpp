#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Game::Graphics
{
    class Polygon;
}

namespace Betomnita::GamePlay
{
    class Terrain final
    {
    public:
        Terrain();
        ~Terrain();

        void Update( const sf::Time& dt );
        void Render( sf::RenderTarget& target );

        void SetFriction( float value )
        {
            m_friction = value;
        }
        float GetFriction() const
        {
            return m_friction;
        }

        void SetShape( std::vector< std::unique_ptr< Game::Graphics::Polygon > >&& shape );

    private:
        float m_friction = 0.0f;
        std::vector< std::unique_ptr< Game::Graphics::Polygon > > m_shape;
    };
}