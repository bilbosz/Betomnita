#include "resource/Resource.hpp"

#include "app/Error.hpp"
#include "project/Config.hpp"

namespace Resource
{
    void Resource::Init()
    {
        debugFont = new sf::Font();
        VERIFY( debugFont->loadFromFile( "res/font/Courier New/cour.ttf" ) );
    }

    void Resource::Dispose() { delete debugFont; }

    const std::wstring WindowTitle = Betomnita_PROJECT L" " Betomnita_VERSION_MAJOR L"." Betomnita_VERSION_MINOR L"." Betomnita_VERSION_PATCH;

    sf::Font* Resource::debugFont;
}