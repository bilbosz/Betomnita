#pragma once
#include "betomnita/gameplay/PhysicalBody.hpp"

#include "game/graphics/Polygon.hpp"

#include <memory>
#include <vector>

namespace Betomnita::GamePlay
{
    class Prototype;

    class VehicleChassis : public PhysicalBody
    {
    public:
        VehicleChassis();
        ~VehicleChassis();
        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );
        void LoadFromPrototype( const Prototype& prototype );

        void SetPosition( const sf::Vector2f& value );
        const sf::Vector2f& GetPosition() const
        {
            return m_position;
        }

        const sf::Vector2f& GetPivot() const
        {
            return m_pivot;
        }

        const sf::Vector2f& GetGunRotatorSlot() const
        {
            return m_gunRotatorSlot;
        }

        void SetTransform( const sf::Transform& transform )
        {
            for( auto& polygon : m_shape )
            {
                polygon.Transform() = transform;
            }
        }

    private:
        sf::Vector2f m_position;
        float m_direction = 0.0f;
        float m_density = 0.0f;
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< sf::Vector2f > m_physicalBodyShape;
        sf::Vector2f m_pivot;
        sf::Vector2f m_gunRotatorSlot;
    };
}