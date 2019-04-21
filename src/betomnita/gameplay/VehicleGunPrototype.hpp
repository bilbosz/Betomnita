#pragma once
#include "betomnita/gameplay/Prototype.hpp"

#include "game/graphics/Polygon.hpp"
#include "game/utils/Direction.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Betomnita::GamePlay
{
    class VehicleGunPrototype : public Prototype
    {
    public:
        friend class VehicleGun;
        VehicleGunPrototype();
        void LoadFromFile( const std::string& filename ) override;

    private:
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< std::vector< sf::Vector2f > > m_physicalBodyShape;
        sf::Vector2f m_gunRotator;
        Game::Utils::Direction m_shotDirection;
    };
}