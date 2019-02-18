#pragma once
#include <SFML/Graphics.hpp>

namespace Game::Utils
{
    struct Direction
    {
        sf::Vector2f Source;
        sf::Vector2f Destination;
    };
}