#include "betomnita/gameplay/Terrain.hpp"

#include "game/graphics/Polygon.hpp"

namespace Betomnita::GamePlay
{
    Terrain::Terrain()
    {
    }

    Terrain::~Terrain()
    {
    }

    void Terrain::SetShape( std::unique_ptr< Game::Graphics::Polygon > shape )
    {
        m_shape = std::move( shape );
    }

    Game::Graphics::Polygon* Terrain::GetShape() const
    {
        return m_shape.get();
    }
}