#include "betomnita/gameplay/Vehicle.hpp"

#include "betomnita/gameplay/World.hpp"

namespace Betomnita::GamePlay
{
    Vehicle::Vehicle()
    {
    }

    void Vehicle::InitPhysics()
    {
        Chassis.InitPhysics();
        Gun.InitPhysics();
    }

    void Vehicle::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        Chassis.Render( target, transform );
        Gun.Render( target, transform );
    }

    void Vehicle::Update( const sf::Time& dt )
    {
        if( m_id == 1 )
        {
            auto physicalBody = Chassis.GetPhysicalBody();
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
                Gun.SetDirection( Gun.GetDirection() - 1.0f * dt.asSeconds() );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) )
            {
                Gun.SetDirection( Gun.GetDirection() + 1.0f * dt.asSeconds() );
            }
        }
        Chassis.Update( dt );
        Gun.Update( dt );
    }
}
