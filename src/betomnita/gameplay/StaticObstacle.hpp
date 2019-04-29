#pragma once
#include "game/graphics/Polygon.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class b2Body;

namespace pugi
{
    class xml_node;
}

namespace Betomnita::GamePlay
{
    class World;

    class StaticObstacle final
    {
    public:
        StaticObstacle( World* world );
        ~StaticObstacle();

        void Update( const sf::Time& dt );
        void Render( sf::RenderTarget& target, const sf::Transform& transform = sf::Transform() );

        void LoadFromSVGNode( const std::string& filename, pugi::xml_document& doc, pugi::xml_node& node, float scale );

        void InitPhysics();

        void SetTransform( const sf::Transform& transform );

        const Game::Utils::AABB& GetAABB() const
        {
            return m_aabb;
        }

    private:
        World* m_world;
        std::vector< Game::Graphics::Polygon > m_shape;
        Game::Utils::AABB m_aabb;

        b2Body* m_physicalBody;
    };
}