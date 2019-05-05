#pragma once
#include "betomnita/gameplay/PhysicalBody.hpp"

#include "game/graphics/Polygon.hpp"
#include "game/utils/Direction.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class b2Body;

namespace Betomnita::GamePlay
{
    class Prototype;
    class Vehicle;

    class VehicleGun : public PhysicalBody
    {
    public:
        VehicleGun();
        ~VehicleGun();
        void Render( sf::RenderTarget& target, const sf::Transform& transform = sf::Transform() );
        void Update( const sf::Time& dt );
        void LoadFromPrototype( const Prototype& prototype );

        void SetDirection( float value )
        {
            m_direction = value;
        }
        float GetDirection() const
        {
            return m_direction;
        }

        void AssignVehicle( Vehicle* value )
        {
            m_vehicle = value;
        }

        Game::Utils::Direction GetShotDirection() const
        {
            Game::Utils::Direction res;
            res.Source = m_transform.transformPoint( m_shotDirection.Source );
            res.Destination = m_transform.transformPoint( m_shotDirection.Destination );
            return res;
        }

        void InitPhysics();

    private:
        std::vector< Game::Graphics::Polygon > m_shape;
        sf::Vector2f m_gunRotator;
        Game::Utils::Direction m_shotDirection;

        float m_direction = 0.0f;
        Vehicle* m_vehicle;
        sf::Transform m_transform;
    };
}