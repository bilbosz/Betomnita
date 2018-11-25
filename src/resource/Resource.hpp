#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Resource
{
    void Init();
    void Dispose();

    enum class StateId
    {
        Undefined = -1,
        MainMenu,
        Gameplay,
        Pause,
        Size,
    };

    extern const std::wstring WindowTitle;
    extern sf::Font* DebugFont;
    extern sf::Font* DefaultFont;
    extern unsigned int DefaultCharacterSize;
    extern float Pi;
}