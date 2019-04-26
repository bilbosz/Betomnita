#include "betomnita/gameplay/VehicleGun.hpp"

#include "betomnita/gameplay/Vehicle.hpp"
#include "betomnita/gameplay/VehicleGunPrototype.hpp"
#include "betomnita/gameplay/World.hpp"

#include <Box2D/Box2D.h>

namespace Betomnita::GamePlay
{
    VehicleGun::VehicleGun()
    {
    }

    VehicleGun::~VehicleGun()
    {
    }

    void VehicleGun::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        sf::Transform r;
        r.combine( transform );
        r.combine( m_transform );
        for( auto& polygon : m_shape )
        {
            polygon.Render( target, r );
        }
        sf::CircleShape c;
        c.setFillColor( sf::Color::Red );
        c.setRadius( 300.0f );
        c.setPosition( { 0.0f, 0.0f } );
        target.draw( c, r );
    }

    void VehicleGun::Update( const sf::Time& dt )
    {
        m_transform = sf::Transform::Identity;

        auto& chassis = m_vehicle->Chassis();
        auto chassisBody = chassis.GetPhysicalBody();
        auto chassisPosition = chassisBody->GetPosition();

        auto chassisAngle = chassisBody->GetAngle();
        m_transform.rotate( ( m_direction + chassisAngle ) * 180.0f / Game::Consts::Pi, m_gunRotator );

        sf::Transform chassisTransform;
        chassisTransform.rotate( chassisAngle );

        m_position = chassis.GetGunRotatorSlot() - m_gunRotator;
        m_position = chassisTransform.transformPoint( m_position );
        m_transform.translate( sf::Vector2f{ chassisPosition.x, chassisPosition.y } + m_position );
    }

    void VehicleGun::LoadFromPrototype( const Prototype& prototype )
    {
        const auto& gunPrototype = static_cast< const VehicleGunPrototype& >( prototype );
        m_shape = gunPrototype.m_shape;
        m_gunRotator = gunPrototype.m_gunRotator;
        m_physicalBodyShape = gunPrototype.m_physicalBodyShape;
        m_shotDirection = gunPrototype.m_shotDirection;
    }

    void VehicleGun::InitPhysics()
    {
        auto& chassis = m_vehicle->Chassis();
        auto chassisBody = chassis.GetPhysicalBody();
        auto chassisPosition = chassisBody->GetPosition();
        m_position = sf::Vector2f{ chassisPosition.x, chassisPosition.y } + chassis.GetGunRotatorSlot() - m_gunRotator;
        m_direction = 0.0f;
    }
}