#pragma once
#include "betomnita/gameplay/Prototype.hpp"

#include "game/graphics/Polygon.hpp"
#include "game/utils/Direction.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Betomnita::GamePlay
{
    class ProjectilePrototype : public Prototype
    {
    public:
        friend class Projectile;
        ProjectilePrototype();
        void LoadFromFile( const std::string& filename ) override;

    private:
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< std::vector< sf::Vector2f > > m_physicalBodyShape;
        Game::Utils::Direction m_direction;
        float m_density;
    };
}