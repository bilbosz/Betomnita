#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"
#include "betomnita/resources/Resources.hpp"
#include "game/graphics/SVGHelper.hpp"

#include <pugixml.hpp>

namespace Betomnita
{
    Cursor::Cursor()
    {
        pugi::xml_document doc;
        VERIFY( doc.load_file( Resources::CursorPath.c_str() ) );

        auto svgNode = doc.child( "svg" );
        auto scaleAttr = svgNode.attribute( "data-scale" );

        auto scale = 1.0f;
        if( scaleAttr )
        {
            scale = scaleAttr.as_float( scale );
        }

        auto& node = svgNode.select_node( "//path" ).node();

        m_shape = Game::Graphics::Polygon::LoadManyFromSVGNode( Resources::CursorPath.c_str(), doc, node, scale );
    }

    Cursor::~Cursor()
    {
    }

    void Cursor::Render( sf::RenderTarget& target )
    {
        for( auto& polygon : m_shape )
        {
            polygon.Render( target );
        }
    }

    void Cursor::SetPosition( const sf::Vector2f& value )
    {
        m_position = value;
        for( auto& polygon : m_shape )
        {
            polygon.SetPosition( m_position );
        }
    }
}