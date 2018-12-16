#include "resource/Resource.hpp"

#include "app/Debug.hpp"
#include "project/Config.hpp"

namespace Resource
{
    void Init()
    {
        DebugFont = new sf::Font();
        VERIFY( DebugFont->loadFromFile( "res/font/Courier New/cour.ttf" ) );

        DefaultFont = new sf::Font();
        VERIFY( DefaultFont->loadFromFile( "res/font/Arimo/Arimo-Regular.ttf" ) );
    }

    void Dispose()
    {
        delete DebugFont;
        delete DefaultFont;
    }

    const std::wstring GameName = Betomnita_PROJECT;

    const std::wstring WindowTitle = Betomnita_PROJECT L" " Betomnita_VERSION_MAJOR L"." Betomnita_VERSION_MINOR L"." Betomnita_VERSION_PATCH;

    sf::Font* DebugFont;
    sf::Font* DefaultFont;

    const sf::Color Background = { 100, 130, 155, 255 };
    const sf::Color Shroud = { 0, 0, 0, 100 };
    const sf::Color MenuTitle = { 240, 240, 240, 255 };
    const sf::Color MenuEntryNormal = { 220, 220, 220, 255 };
    const sf::Color MenuEntrySelected = { 255, 255, 255, 255 };

    unsigned int DefaultCharacterSize = 30;

    float Pi = 2.0f * asinf( 1.0f );

    const std::string ScreenshotPath = "screenshot.png";
}