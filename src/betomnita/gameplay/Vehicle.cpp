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
        if( m_id == 1 )
        {
            auto physicalBody = m_chassis.GetPhysicalBody();
            float impulse = 370'000.0f * dt.asSeconds();
            auto angle = physicalBody->GetAngle() + Game::Consts::Pi * 0.5f;
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
            {
                physicalBody->ApplyLinearImpulseToCenter( b2Vec2( -impulse * cosf( angle ), -impulse * sinf( angle ) ), true );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
            {
                physicalBody->ApplyLinearImpulseToCenter( b2Vec2( impulse * cosf( angle ), impulse * sinf( angle ) ), true );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
            {
                physicalBody->ApplyAngularImpulse( -impulse, true );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
            {
                physicalBody->ApplyAngularImpulse( impulse, true );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Num0 ) )
            {
                physicalBody->SetAngularVelocity( 0.0f );
                physicalBody->SetLinearVelocity( b2Vec2( 0.0f, 0.0f ) );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) )
            {
                m_gun.SetDirection( m_gun.GetDirection() - 1.0f * dt.asSeconds() );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) )
            {
                m_gun.SetDirection( m_gun.GetDirection() + 1.0f * dt.asSeconds() );
            }
        }
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
