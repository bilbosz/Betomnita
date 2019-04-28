#include "betomnita/gameplay/VehicleGun.hpp"

#include "betomnita/gameplay/Vehicle.hpp"
#include "betomnita/gameplay/VehicleGunPrototype.hpp"
#include "betomnita/gameplay/World.hpp"
#include "game/utils/Utils.hpp"

#include <Box2D/Box2D.h>

namespace Betomnita::GamePlay
{
    using Game::Utils::cast;

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
    }

    void VehicleGun::Update( const sf::Time& dt )
    {
        m_transform = sf::Transform::Identity;

        auto& chassis = m_vehicle->Chassis();
        auto chassisBody = chassis.GetPhysicalBody();
        auto chassisAngle = chassisBody->GetAngle();

        auto gunRotatorSlot = cast< sf::Vector2f >( chassisBody->GetWorldPoint( cast< b2Vec2 >( chassis.GetGunRotatorSlot() ) ) );
        m_transform.translate( gunRotatorSlot - m_gunRotator );
        m_transform.rotate( ( chassisAngle + m_direction ) * Game::Consts::RadToDeg, m_gunRotator );
    }

    void VehicleGun::LoadFromPrototype( const Prototype& prototype )
    {
        const auto& gunPrototype = static_cast< const VehicleGunPrototype& >( prototype );
        m_shape = gunPrototype.m_shape;
        m_gunRotator = gunPrototype.m_gunRotator;
        m_shotDirection = gunPrototype.m_shotDirection;
    }

    void VehicleGun::InitPhysics()
    {
        auto& chassis = m_vehicle->Chassis();
        auto chassisBody = chassis.GetPhysicalBody();
        auto chassisPosition = chassisBody->GetPosition();
        m_position = cast< sf::Vector2f >( chassisPosition ) + chassis.GetGunRotatorSlot() - m_gunRotator;
        m_direction = 0.0f;
    }
}