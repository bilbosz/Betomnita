#include "betomnita/gameplay/Terrain.hpp"

#include "app/Debug.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"

#include <pugixml.hpp>

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

    void Terrain::LoadFromSVGNode( const std::string& filename, const pugi::xml_node& node, float scale )
    {
        m_shape = Game::Graphics::Polygon::LoadManyFromSVGNode( filename, node, scale );
        m_friction = node.attribute( "data-friction" ).as_float();
    }
}