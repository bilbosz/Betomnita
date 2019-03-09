#include "betomnita/gameplay/Terrain.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/World.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"

#include <pugixml.hpp>

namespace Betomnita::GamePlay
{
    Terrain::Terrain( const World* world ) : m_world( world )
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
            shape.Render( target );
        }
    }

    void Terrain::LoadFromSVGNode( const std::string& filename, pugi::xml_document& doc, pugi::xml_node& node, float scale )
    {
        m_shape = Game::Graphics::Polygon::LoadManyFromSVGNode( filename, doc, node, scale );
        m_friction = node.attribute( "data-friction" ).as_float();
    }

    void Terrain::SetTransform( const sf::Transform& transform )
    {
        for( auto& shape : m_shape )
        {
            shape.Transform() = transform;
        }
    }
}