#include "betomnita/gameplay/World.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/PhysicalBody.hpp"

namespace Betomnita::GamePlay
{
    World::World()
    {
        m_physicalBodies.emplace_back( std::make_unique< PhysicalBody >() );
    }

    World::~World()
    {
    }

    void World::Render( sf::RenderTarget& target )
    {
    }

    void World::Update( const sf::Time& dt )
    {
    }
}