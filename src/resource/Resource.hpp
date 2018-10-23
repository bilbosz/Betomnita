#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Resource
{
    void Init();
    void Dispose();

    extern const std::wstring WindowTitle;
    extern sf::Font* DebugFont;
    extern unsigned int DefaultCharacterSize;
}