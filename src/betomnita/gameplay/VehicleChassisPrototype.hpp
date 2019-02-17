#pragma once
#include "betomnita/gameplay/Prototype.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Game::Graphics
{
    class Polygon;
}

namespace Betomnita::GamePlay
{
    class VehicleChassisPrototype : public Prototype
    {
    public:
        friend class World;
        VehicleChassisPrototype();
        void LoadFromFile( const std::string& filename ) override;

    private:
        std::vector< std::unique_ptr< Game::Graphics::Polygon > > m_shape;
        std::vector< sf::Vector2f > m_physicalBodyShape;
        sf::Vector2f m_pivot;
        sf::Vector2f m_gunRotatorSlot;
    };
}