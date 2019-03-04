#pragma once
#include "app/Debug.hpp"
#include "betomnita/gameplay/VehicleChassis.hpp"
#include "betomnita/gameplay/VehicleGun.hpp"

#include <SFML/Graphics.hpp>

namespace Betomnita::GamePlay
{
    class Vehicle
    {
    public:
        Vehicle();
        bool operator<( const Vehicle& other ) const
        {
            ASSERT( m_id >= 0 && other.m_id >= 0, L"Id should be positive number" );
            return m_id < other.m_id;
        }

        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );

        int GetId() const
        {
            ASSERT( m_id >= 0, L"Id should be positive number" );
            return m_id;
        }
        void SetId( int value )
        {
            m_id = value;
        }

        void SetDirection( float value )
        {
            m_direction = value;
        }
        float GetDirection() const
        {
            return m_direction;
        }

        void SetPosition( const sf::Vector2f& value )
        {
            m_position = value;
            m_chassis.SetPosition( m_position );
            m_gun.SetPosition( m_position + m_chassis.GetGunRotatorSlot() );
        }
        const sf::Vector2f& GetPosition() const
        {
            return m_position;
        }

        VehicleChassis& Chassis();
        VehicleGun& Gun();

    private:
        int m_id = -1;
        float m_direction = 0.0f;
        sf::Vector2f m_position;
        VehicleChassis m_chassis;
        VehicleGun m_gun;
    };
}