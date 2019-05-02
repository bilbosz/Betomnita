#pragma once
#include "app/Debug.hpp"
#include "betomnita/gameplay/VehicleChassis.hpp"
#include "betomnita/gameplay/VehicleGun.hpp"
#include "game/GameConsts.hpp"

#include <SFML/Graphics.hpp>

class b2Body;

namespace Betomnita::GamePlay
{
    class World;

    class Vehicle
    {
    public:
        Vehicle();
        bool operator<( const Vehicle& other ) const
        {
            return m_id < other.m_id;
        }

        void InitPhysics();

        void Render( sf::RenderTarget& target, const sf::Transform& transform = sf::Transform() );
        void Update( const sf::Time& dt );

        int GetId() const
        {
            return m_id;
        }
        void SetId( int value )
        {
            m_id = value;
            ASSERT( m_id >= 0, L"Id should be non-negative number" );
        }

        VehicleChassis Chassis;
        VehicleGun Gun;
        GamePlay::World* World;

    private:
        int m_id = -1;
    };
}