#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Betomnita::Resources
{
    void Init();
    void Dispose();

    enum class StateId
    {
        Undefined = -1,
        MainMenu,
        GamePlay,
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
        Undefined = -1,
        Default,
        MainMenuClick,
        MainMenuHover,
        PauseRequest,
        PauseMenuClick,
        PauseMenuHover,
        Size,
    };

    extern const std::wstring GameName;
    extern const std::wstring WindowTitle;
    extern sf::Font* DebugFont;
    extern sf::Font* DefaultFont;
    extern const sf::Color Background;
    extern const sf::Color Shroud;
    extern const sf::Color MenuTitle;
    extern const sf::Color MenuEntryNormal;
    extern const sf::Color MenuEntrySelected;
    extern const std::string ScreenshotPath;
    extern const std::string CursorPath;
    extern const std::string World1;
}