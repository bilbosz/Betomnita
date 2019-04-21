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
    }

    void VehicleGun::Update( const sf::Time& dt )
    {
        float angle = m_physicalBody->GetAngle();
        auto pos = m_physicalBody->GetPosition();
        m_physicalBody->SetTransform( pos, m_vehicle->Chassis().GetPhysicalBody()->GetAngle() );
        sf::Vector2f position( pos.x, pos.y );
        m_transform = sf::Transform::Identity;
        m_transform.translate( position );
        m_transform.rotate( angle * 180.0f / Game::Consts::Pi );
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
        auto& gunRotatorPosition = chassis.GetPhysicalBody()->GetWorldPoint( { chassis.GetGunRotatorSlot().x, chassis.GetGunRotatorSlot().y } );
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set( gunRotatorPosition.x, gunRotatorPosition.y );

        auto& physicsWorld = m_vehicle->World()->PhysicsWorld();
        m_physicalBody = physicsWorld.CreateBody( &bodyDef );

        b2PolygonShape shape;
        b2FixtureDef fixtureDef;
        fixtureDef.density = 0.0f;
        fixtureDef.filter.maskBits = 0x0000;
        for( const auto& triangle : m_physicalBodyShape )
        {
            b2Vec2 points[ 3 ] = {
                { triangle[ 0 ].x, triangle[ 0 ].y },
                { triangle[ 1 ].x, triangle[ 1 ].y },
                { triangle[ 2 ].x, triangle[ 2 ].y },
            };
            shape.Set( points, 3 );
            fixtureDef.shape = &shape;
            m_physicalBody->CreateFixture( &fixtureDef );
        }

        b2RevoluteJointDef jointDef;
        jointDef.bodyA = chassis.GetPhysicalBody();
        jointDef.localAnchorA = { chassis.GetGunRotatorSlot().x, chassis.GetGunRotatorSlot().y };
        jointDef.bodyB = m_physicalBody;
        jointDef.localAnchorB = { m_gunRotator.x, m_gunRotator.y };
        jointDef.collideConnected = false;
        physicsWorld.CreateJoint( &jointDef );
    }
}