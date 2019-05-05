#pragma once
#include "betomnita/gameplay/PhysicalBody.hpp"

#include "game/graphics/Polygon.hpp"
#include "game/utils/Direction.hpp"

#include <memory>
#include <vector>

class b2Body;

namespace Betomnita::GamePlay
{
    class Prototype;
    class World;
    class Vehicle;

    class Projectile : public PhysicalBody
    {
    public:
        Projectile();
        ~Projectile();
        void Render( sf::RenderTarget& target, const sf::Transform& transform = sf::Transform() );
        void Update( const sf::Time& dt );
        void LoadFromPrototype( const Prototype& prototype );
        void InitPhysics();
        void UpdateTransformation();

        void SetInitialPosition( const sf::Vector2f& value )
        {
            m_initialPosition = value;
        }

        void SetInitialAngle( float value )
        {
            m_initialAngle = value;
        }

        float GetDensity() const
        {
            return m_density;
        }

        b2Body* GetPhysicalBody()
        {
            return m_physicalBody;
        }

        Game::Utils::Direction GetShotDirection() const
        {
            Game::Utils::Direction res;
            res.Source = m_transform.transformPoint( m_direction.Source );
            res.Destination = m_transform.transformPoint( m_direction.Destination );
            return res;
        }

        void AssignShooter( Vehicle* vehicle )
        {
            m_shooter = vehicle;
        }

        void SetToDestroy()
        {
            m_setToDestroy = true;
        }
        bool IsSetToDestroy() const
        {
            return m_setToDestroy;
        }

        GamePlay::World* World;

    private:
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< std::vector< sf::Vector2f > > m_physicalBodyShape;
        Game::Utils::Direction m_direction;
        float m_density;

        sf::Vector2f m_initialPosition;
        float m_initialAngle;
        Vehicle* m_shooter;
        b2Body* m_physicalBody;
        sf::Transform m_transform;
        float m_speed = 0.0f;
        bool m_setToDestroy = false;
    };
}