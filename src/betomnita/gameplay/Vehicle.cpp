#include "betomnita/gameplay/Vehicle.hpp"

namespace Betomnita::GamePlay
{
    Vehicle::Vehicle()
    {
    }

    void Vehicle::Render( sf::RenderTarget& target )
    {
        m_chassis.Render( target );
        m_gun.Render( target );
    }

    void Vehicle::Update( const sf::Time& dt )
    {
        m_chassis.Update( dt );
        m_gun.Update( dt );
    }

    VehicleChassis& Vehicle::Chassis()
    {
        return m_chassis;
    }

    VehicleGun& Vehicle::Gun()
    {
        return m_gun;
    }
}
