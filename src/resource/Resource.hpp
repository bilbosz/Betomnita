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

    enum class EventId
    {
        Undefined = -1,
        OnStart,
        OnUpdate,
        OnRender,
        OnVideoSettingChanged,
        OnClose,
        OnKeyPressed,
        OnKeyReleased,
        OnMouseButtonPressed,
        OnMouseButtonReleased,
        OnMouseMoved,
        Size,
    };

    enum class ListenerId
    {
        Undefined  = -1,
        Default,
        MainMenuClick,
        Size,
    };

    extern const std::wstring GameName;
    extern const std::wstring WindowTitle;
    extern sf::Font* DebugFont;
    extern sf::Font* DefaultFont;
    extern unsigned int DefaultCharacterSize;
    extern float Pi;
    extern const std::string ScreenshotPath;
}