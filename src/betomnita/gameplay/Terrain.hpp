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
    class Terrain
    {
    public:
        Terrain();
        ~Terrain();

        void SetFriction( float value )
        {
            m_friction = value;
        }
        float GetFriction() const
        {
            return m_friction;
        }

        void SetShape( std::unique_ptr< Game::Graphics::Polygon > shape );
        Game::Graphics::Polygon* GetShape() const;

    private:
        float m_friction = 0.0f;
        std::unique_ptr< Game::Graphics::Polygon > m_shape;
    };
}