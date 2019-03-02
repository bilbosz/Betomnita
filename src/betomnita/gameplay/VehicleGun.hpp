#pragma once
#include "betomnita/gameplay/PhysicalBody.hpp"

#include "game/graphics/Polygon.hpp"
#include "game/utils/Direction.hpp"

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

    private:
        std::vector< Game::Graphics::Polygon > m_shape;
        std::vector< sf::Vector2f > m_physicalBodyShape;
        sf::Vector2f m_gunRotator;
        Game::Utils::Direction m_shotDirection;
    };
}