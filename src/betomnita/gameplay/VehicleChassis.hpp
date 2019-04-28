#pragma once
#include "betomnita/gameplay/PhysicalBody.hpp"

#include "game/graphics/Polygon.hpp"

#include <memory>
#include <vector>

class b2Body;

namespace Betomnita::GamePlay
{
    class Prototype;
    class Vehicle;

    class VehicleChassis : public PhysicalBody
    {
    public:
        VehicleChassis();
        ~VehicleChassis();
        void Render( sf::RenderTarget& target, const sf::Transform& transform = sf::Transform() );
        void Update( const sf::Time& dt );
        void LoadFromPrototype( const Prototype& prototype );
        void InitPhysics();
        void UpdatePhysics();
        void UpdateTransformation();


        void AssignVehicle( Vehicle* vehicle );

        void SetInitialPosition( const sf::Vector2f& value )
        {
            m_initialPosition = value;
        }

        const sf::Vector2f& GetGunRotatorSlot() const
        {
            return m_gunRotatorSlot;
        }

        float GetDensity() const
        {
            return m_density;
        }

        b2Body* GetPhysicalBody()
        {
            return m_physicalBody;
        }

    private:
        float m_density = 0.0f;
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< std::vector< sf::Vector2f > > m_physicalBodyShape;
        sf::Vector2f m_gunRotatorSlot;

        sf::Vector2f m_initialPosition;
        Vehicle* m_vehicle;
        b2Body* m_physicalBody;
        sf::Transform m_transform;
    };
}