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

    const std::wstring WindowTitle = Betomnita_PROJECT L" " Betomnita_VERSION_MAJOR L"." Betomnita_VERSION_MINOR L"." Betomnita_VERSION_PATCH;

    sf::Font* DebugFont;
    sf::Font* DefaultFont;

    unsigned int DefaultCharacterSize = 30;
}