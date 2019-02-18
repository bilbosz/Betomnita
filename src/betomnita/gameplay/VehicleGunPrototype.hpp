#pragma once
#include "betomnita/gameplay/Prototype.hpp"

#include "game/utils/Direction.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Game::Graphics
{
    class Polygon;
}

namespace Betomnita::GamePlay
{
    class VehicleGunPrototype : public Prototype
    {
    public:
        VehicleGunPrototype();

        void LoadFromFile( const std::string& filename ) override;

    private:
        std::vector< std::unique_ptr< Game::Graphics::Polygon > > m_shape;
        std::vector< sf::Vector2f > m_physicalBodyShape;
        sf::Vector2f m_gunRotator;
        Game::Utils::Direction m_shotDirection;
    };
}