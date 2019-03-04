#include "betomnita/gameplay/VehicleChassis.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/VehicleChassisPrototype.hpp"
#include "game/GameConsts.hpp"
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
        for( auto& polygon : m_shape )
        {
            polygon.Render( target );
        }
    }

    void VehicleChassis::Update( const sf::Time& dt )
    {
    }

    void VehicleChassis::LoadFromPrototype( const Prototype& prototype )
    {
        const auto& vehiclePrototype = static_cast< const VehicleChassisPrototype& >( prototype );
        m_pivot = vehiclePrototype.m_pivot;
        m_gunRotatorSlot = vehiclePrototype.m_gunRotatorSlot;
        m_physicalBodyShape = vehiclePrototype.m_physicalBodyShape;
        m_shape = vehiclePrototype.m_shape;
    }

    void VehicleChassis::SetPosition( const sf::Vector2f& value )
    {
        m_position = value;
        for( auto& polygon : m_shape )
        {
            polygon.SetPosition( m_position );
        }
    }
}
