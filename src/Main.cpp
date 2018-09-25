#include "app/Error.hpp"
#include "game/Game.hpp"
#include "project/Config.hpp"

#include <iostream>

int main( int argc, char* argv[] )
{
    Resource::Init();
    Game::Game game;
#ifdef FULLSCREEN
    game.SetFullscreen();
#else
    game.SetBorderlessWindow();
    game.SetVideoMode( sf::VideoMode( 640, 480, 32 ) );
#endif
    game.Run();
    Resource::Dispose();
#ifdef CONSOLE_STOP
    std::getchar();
#endif
}
