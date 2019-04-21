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
        explicit Vehicle( World* world );
        bool operator<( const Vehicle& other ) const
        {
            ASSERT( m_id >= 0 && other.m_id >= 0, L"Id should be non-negative number" );
            return m_id < other.m_id;
        }

        void InitPhysics();

        void Render( sf::RenderTarget& target, const sf::Transform& transform = sf::Transform() );
        void Update( const sf::Time& dt );

        int GetId() const
        {
            ASSERT( m_id >= 0, L"Id should be non-negative number" );
            return m_id;
        }
        void SetId( int value )
        {
            m_id = value;
        }

        VehicleChassis& Chassis();
        VehicleGun& Gun();
        GamePlay::World* World();

    private:
        GamePlay::World* m_world;
        int m_id = -1;
        VehicleChassis m_chassis;
        VehicleGun m_gun;
    };
}