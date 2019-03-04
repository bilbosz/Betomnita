#pragma once
#include "betomnita/gameplay/PhysicalBody.hpp"

#include "game/graphics/Polygon.hpp"
#include "game/utils/Direction.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Betomnita::GamePlay
{
    class Prototype;

    class VehicleGun : public PhysicalBody
    {
    public:
        VehicleGun();
        ~VehicleGun();
        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );
        void LoadFromPrototype( const Prototype& prototype );

        void SetPosition( const sf::Vector2f& value );
        const sf::Vector2f& GetPosition() const
        {
            return m_position;
        }

    private:
        float m_direction = 0.0f;
        sf::Vector2f m_position;
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< sf::Vector2f > m_physicalBodyShape;
        sf::Vector2f m_gunRotator;
        Game::Utils::Direction m_shotDirection;
    };
}