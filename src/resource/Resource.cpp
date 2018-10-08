#include "resource/Resource.hpp"

#include "app/Debug.hpp"
#include "project/Config.hpp"

namespace Resource
{
    void Init()
    {
        DebugFont = new sf::Font();
        VERIFY( DebugFont->loadFromFile( "res/font/Courier New/cour.ttf" ) );
    }

    void Dispose()
    {
        delete DebugFont;
    }

    const std::wstring WindowTitle = Betomnita_PROJECT L" " Betomnita_VERSION_MAJOR L"." Betomnita_VERSION_MINOR L"." Betomnita_VERSION_PATCH;

    sf::Font* DebugFont;
}