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
#ifdef FULLSCREEN
    game.EnableVerticalSync();
    game.SetFullscreen();
    game.HideCursor();
#else
    game.SetFramerateLimit( 0 );
    game.SetBorderedWindow( false, true );
    game.ReleaseCursor();
    game.HideCursor();
    game.SetVideoMode( sf::VideoMode( 1024, 768, 32 ) );
#endif
    game.Run();
    Betomnita::Resources::Dispose();
#ifdef CONSOLE_STOP
    std::getchar();
#endif
}
