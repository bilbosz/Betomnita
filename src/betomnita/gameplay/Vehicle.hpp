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

        VehicleChassis& Chassis();
        VehicleGun& Gun();

    private:
        int m_id = -1;
        VehicleChassis m_chassis;
        VehicleGun m_gun;
    };
}