#include "betomnita/gameplay/VehicleChassis.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/Vehicle.hpp"
#include "betomnita/gameplay/VehicleChassisPrototype.hpp"
#include "betomnita/gameplay/World.hpp"
#include "game/GameConsts.hpp"
#include "game/graphics/Polygon.hpp"

#include "Box2D/Box2D.h"

namespace Betomnita::GamePlay
{
    VehicleChassis::VehicleChassis()
    {
    }

    VehicleChassis::~VehicleChassis()
    {
    }

    void VehicleChassis::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        sf::Transform r;
        r.combine( transform );
        r.combine( m_transform );
        for( auto& polygon : m_shape )
        {
            polygon.Render( target, r );
        }
    }

    void VehicleChassis::Update( const sf::Time& dt )
    {
        float angle = m_physicalBody->GetAngle();
        auto pos = m_physicalBody->GetPosition();
        sf::Vector2f position( pos.x, pos.y );
        m_transform = sf::Transform::Identity;
        m_transform.translate( position );
        m_transform.rotate( angle * 180.0f / Game::Consts::Pi );
    }

    void VehicleChassis::LoadFromPrototype( const Prototype& prototype )
    {
        const auto& vehiclePrototype = static_cast< const VehicleChassisPrototype& >( prototype );
        m_gunRotatorSlot = vehiclePrototype.m_gunRotatorSlot;
        m_physicalBodyShape = vehiclePrototype.m_physicalBodyShape;
        m_shape = vehiclePrototype.m_shape;
        m_density = vehiclePrototype.m_density;
    }

    void VehicleChassis::InitPhysics()
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set( m_initialPosition.x, m_initialPosition.y );

        auto& physicsWorld = m_vehicle->World()->PhysicsWorld();
        m_physicalBody = physicsWorld.CreateBody( &bodyDef );

        b2PolygonShape shape;
        for( const auto& triangle : m_physicalBodyShape )
        {
            b2Vec2 points[ 3 ] = {
                { triangle[ 0 ].x, triangle[ 0 ].y },
                { triangle[ 1 ].x, triangle[ 1 ].y },
                { triangle[ 2 ].x, triangle[ 2 ].y },
            };
            shape.Set( points, 3 );
            m_physicalBody->CreateFixture( &shape, m_density );
        }
        m_physicalBody->SetLinearDamping( 0.28f );
        m_physicalBody->SetAngularDamping( 2.0f );
    }

    void VehicleChassis::AssignVehicle( Vehicle* vehicle )
    {
        m_vehicle = vehicle;
    }
}
