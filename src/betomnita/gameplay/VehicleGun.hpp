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

        void AssignVehicle( Vehicle* value )
        {
            m_vehicle = value;
        }

        void InitPhysics();

    private:
        float m_direction = 0.0f;
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< std::vector< sf::Vector2f > > m_physicalBodyShape;
        sf::Vector2f m_gunRotator;
        Game::Utils::Direction m_shotDirection;

        Vehicle* m_vehicle;
        b2Body* m_physicalBody;
        sf::Transform m_transform;
        float m_angle = 0.0f;
    };
}