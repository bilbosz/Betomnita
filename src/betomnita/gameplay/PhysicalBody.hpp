#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace Game::Graphics
{
    class Polygon;
}

namespace Betomnita::GamePlay
{
    class PhysicalBody
    {
    public:
        PhysicalBody();
        ~PhysicalBody();

    private:
        std::vector< sf::Vector2f > m_shape;
        std::unique_ptr< Game::Graphics::Polygon > m_graphics;
    };
}