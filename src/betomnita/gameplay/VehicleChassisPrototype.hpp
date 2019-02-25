#pragma once
#include "betomnita/gameplay/Prototype.hpp"

#include "game/graphics/Polygon.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Betomnita::GamePlay
{
    class VehicleChassisPrototype : public Prototype
    {
    public:
        friend class VehicleChassis;
        VehicleChassisPrototype();
        void LoadFromFile( const std::string& filename ) override;

    private:
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< sf::Vector2f > m_physicalBodyShape;
        sf::Vector2f m_pivot;
        sf::Vector2f m_gunRotatorSlot;
    };
}