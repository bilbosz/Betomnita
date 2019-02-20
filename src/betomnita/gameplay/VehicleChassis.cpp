#include "betomnita/gameplay/VehicleChassis.hpp"

#include "betomnita/gameplay/VehicleChassisPrototype.hpp"
#include "game/graphics/Polygon.hpp"

namespace Betomnita::GamePlay
{
    VehicleChassis::VehicleChassis()
    {
    }

    VehicleChassis::~VehicleChassis()
    {
    }

    void VehicleChassis::Render( sf::RenderTarget& target )
    {
    }

    void VehicleChassis::Update( const sf::Time& dt )
    {
    }

    void VehicleChassis::LoadFromPrototype( const VehicleChassisPrototype& prototype )
    {
        for( const auto& polygon : prototype.m_shape )
        {
            prototype.m_shape.push_back( std::make_unique< Game::Graphics::Polygon >( *polygon ) );
        }
    }
}
