#include "betomnita/gameplay/Terrain.hpp"

#include "app/Debug.hpp"
#include "game/graphics/Polygon.hpp"

namespace Betomnita::GamePlay
{
    Terrain::Terrain()
    {
    }

    Terrain::~Terrain()
    {
    }

    void Terrain::Update( const sf::Time& dt )
    {
    }

    void Terrain::Render( sf::RenderTarget& target )
    {
        for( auto& shape : m_shape )
        {
            shape->Render( target );
        }
    }

    void Terrain::SetShape( std::vector< std::unique_ptr< Game::Graphics::Polygon > >&& shape )
    {
        m_shape = std::move( shape );
    }
}