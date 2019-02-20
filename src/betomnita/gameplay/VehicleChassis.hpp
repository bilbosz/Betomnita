#pragma once
#include "betomnita/gameplay/PhysicalBody.hpp"

#include <memory>
#include <vector>

namespace Betomnita::GamePlay
{
    class VehicleChassisPrototype;

    class VehicleChassis : public PhysicalBody
    {
    public:
        VehicleChassis();
        ~VehicleChassis();
        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );
        void LoadFromPrototype( const VehicleChassisPrototype& prototype );

    private:
        sf::Vector2f m_position = { 0.0f, 0.0f };
        float m_direction = 0.0f;
        std::vector< std::unique_ptr< Game::Graphics::Polygon > > m_shape;
        std::vector< sf::Vector2f > m_physicalBodyShape;
        sf::Vector2f m_pivot;
        sf::Vector2f m_gunRotatorSlot;
    };
}