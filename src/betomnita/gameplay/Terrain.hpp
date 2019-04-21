#pragma once
#include "game/graphics/Polygon.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace pugi
{
    class xml_node;
}

namespace Betomnita::GamePlay
{
    class World;

    class Terrain final
    {
    public:
        Terrain( const World* world );
        ~Terrain();

        void Update( const sf::Time& dt );
        void Render( sf::RenderTarget& target, const sf::Transform& transform = sf::Transform() );

        float GetFriction() const
        {
            return m_friction;
        }

        void LoadFromSVGNode( const std::string& filename, pugi::xml_document& doc, pugi::xml_node& node, float scale );

        void SetTransform( const sf::Transform& transform );

        const Game::Utils::AABB& GetAABB() const
        {
            return m_aabb;
        }

    private:
        const World* m_world;
        float m_friction = 0.0f;
        std::vector< Game::Graphics::Polygon > m_shape;
        Game::Utils::AABB m_aabb;
    };
}