#include "ProjectConfig.hpp"

#include <iostream>

int main( int argc, char* argv[] )
{
    std::cout << "Welcome to " << Betomnita_PROJECT << " " << Betomnita_VERSION_MAJOR << "." << Betomnita_VERSION_MINOR
              << "." << Betomnita_VERSION_PATCH << std::endl;

#ifdef CONSOLE_STOP
    std::getchar();
#endif
}
