#include "betomnita/gameplay/Projectile.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/ProjectilePrototype.hpp"
#include "betomnita/gameplay/World.hpp"
#include "betomnita/resources/Resources.hpp"
#include "game/GameConsts.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/utils/Utils.hpp"

#include <Box2D/Box2D.h>

namespace Betomnita::GamePlay
{
    Projectile::Projectile()
    {
    }

    Projectile::~Projectile()
    {
        World->PhysicsWorld().DestroyBody( m_physicalBody );
    }

    void Projectile::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        sf::Transform r;
        r.combine( transform );
        r.combine( m_transform );
        for( auto& polygon : m_shape )
        {
            polygon.Render( target, r );
        }
    }

    void Projectile::Update( const sf::Time& dt )
    {
        UpdateTransformation();
        auto v = Game::Utils::Cast< sf::Vector2f >( m_physicalBody->GetLinearVelocity() );
        m_speed = std::max( m_speed, hypotf( v.x, v.y ) );
    }

    void Projectile::LoadFromPrototype( const Prototype& prototype )
    {
        const auto& projectilePrototype = static_cast< const ProjectilePrototype& >( prototype );
        m_direction = projectilePrototype.m_direction;
        m_physicalBodyShape = projectilePrototype.m_physicalBodyShape;
        m_shape = projectilePrototype.m_shape;
        m_density = projectilePrototype.m_density;
    }

    void Projectile::InitPhysics()
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.bullet = true;
        bodyDef.position.Set( m_initialPosition.x, m_initialPosition.y );
        bodyDef.angle = m_initialAngle;

        auto& physicsWorld = World->PhysicsWorld();
        m_physicalBody = physicsWorld.CreateBody( &bodyDef );

        b2FixtureDef fixtureDef;
        fixtureDef.density = m_density;
        fixtureDef.restitution = 0.0f;

        b2PolygonShape shape;
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

        m_physicalBody->SetLinearDamping( 0.0f );
        m_physicalBody->SetAngularDamping( 0.0f );
    }

    void Projectile::UpdateTransformation()
    {
        m_transform = sf::Transform::Identity;
        m_transform.translate( Game::Utils::Cast< sf::Vector2f >( m_physicalBody->GetPosition() ) );
        m_transform.rotate( m_physicalBody->GetAngle() * Game::Consts::RadToDeg );
    }
}
