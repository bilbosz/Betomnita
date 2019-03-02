#include "betomnita/gameplay/VehicleGun.hpp"

#include "betomnita/gameplay/VehicleGunPrototype.hpp"

namespace Betomnita::GamePlay
{
    VehicleGun::VehicleGun()
    {
    }

    VehicleGun::~VehicleGun()
    {
    }

    void VehicleGun::Render( sf::RenderTarget& target )
    {
        for( auto& polygon : m_shape )
        {
            polygon.Render( target );
        }
    }

    void VehicleGun::Update( const sf::Time& dt )
    {
    }

    void VehicleGun::LoadFromPrototype( const Prototype& prototype )
    {
        const auto& gunPrototype = static_cast< const VehicleGunPrototype& >( prototype );
        m_shape = gunPrototype.m_shape;
        m_gunRotator = gunPrototype.m_gunRotator;
        m_physicalBodyShape = gunPrototype.m_physicalBodyShape;
        m_shotDirection = gunPrototype.m_shotDirection;
    }
}