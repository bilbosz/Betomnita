#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "project/Config.hpp"

#include <iostream>

int main( int argc, char* argv[] )
{
    Resource::Init();
    Betomnita::BetomnitaGame game;
#ifdef FULLSCREEN
    game.EnableVerticalSync();
    game.SetFullscreen();
	game.HideCursor();
#else
    game.SetBorderedWindow( false, true );
    game.ReleaseCursor();
	game.HideCursor();
    game.SetVideoMode( sf::VideoMode( 1024, 768, 32 ) );
#endif
    game.Run();
    Resource::Dispose();
#ifdef CONSOLE_STOP
    std::getchar();
#endif
}
