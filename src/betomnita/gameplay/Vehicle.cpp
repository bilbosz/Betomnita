#include "betomnita/gameplay/Vehicle.hpp"

#include "betomnita/gameplay/World.hpp"

namespace Betomnita::GamePlay
{
    Vehicle::Vehicle( GamePlay::World* world ) : m_world( world )
    {
    }

    void Vehicle::InitPhysics()
    {
        Chassis().InitPhysics();
        Gun().InitPhysics();
    }

    void Vehicle::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        m_chassis.Render( target, transform );
        m_gun.Render( target, transform );
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

    GamePlay::World* Vehicle::World()
    {
        return m_world;
    }
}
