#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace pugi
{
    class xml_node;
}

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

        float GetFriction() const
        {
            return m_friction;
        }

        void LoadFromSVGNode( const std::string& filename, const pugi::xml_node& node, float scale );

    private:
        float m_friction = 0.0f;
        std::vector< std::unique_ptr< Game::Graphics::Polygon > > m_shape;
    };
}