#include "betomnita/gameplay/PhysicalBody.hpp"

#include "game/graphics/Polygon.hpp"

#include <Box2D/Box2D.h>

namespace Betomnita::GamePlay
{
    PhysicalBody::PhysicalBody() //: m_graphics( std::make_unique< Game::Graphics::Polygon >() )
    {
    }

    PhysicalBody::~PhysicalBody()
    {
    }
}