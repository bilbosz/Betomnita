#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/resources/Resources.hpp"
#include "project/Config.hpp"

#include <iostream>

int main( int argc, char* argv[] )
{
    Betomnita::Resources::Init();
    Betomnita::BetomnitaGame game;
    game.SetWindowTitle( Betomnita::Resources::WindowTitle );
    game.SetFramerateLimit( 0 );
    game.HideCursor();
#ifdef FULLSCREEN
    game.SetFullscreen();
#else
    game.SetBorderedWindow( false, true );
    game.ReleaseCursor();

    const auto desktopVideoMode = sf::VideoMode::getDesktopMode();
    const auto videoMode = sf::VideoMode( 1280, 800 );
    game.SetVideoMode( videoMode );
    sf::Vector2i windowPosition = { static_cast< int >( ( desktopVideoMode.width - videoMode.width ) * 0.5f ),
                                    static_cast< int >( ( desktopVideoMode.height - videoMode.height ) * 0.5f ) };
    game.SetWindowPosition( windowPosition );
#endif
    game.Run();
    Betomnita::Resources::Dispose();
#ifdef CONSOLE_STOP
    std::getchar();
#endif
}
