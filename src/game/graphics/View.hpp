#pragma once
#include <SFML/Graphics.hpp>

namespace Game::Graphics
{
    struct View
    {
        sf::Vector2f Center = { 0.0f, 0.0f };
        float Rotation = 0.0f;
        float Scale = 1.0f;

        sf::Transform GetTransform() const;
    };
}