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
        m_pivot = prototype.m_pivot;
        m_gunRotatorSlot = prototype.m_gunRotatorSlot;
        m_physicalBodyShape = prototype.m_physicalBodyShape;
        m_shape = prototype.m_shape;
    }
}
