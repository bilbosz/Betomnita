#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"
#include "betomnita/resources/Resources.hpp"
#include "game/graphics/Polygon.hpp"
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

        const auto& node = svgNode.select_node( "//path" ).node();
        const auto nodeName = node.name();

        m_shape = std::make_unique< Game::Graphics::Polygon >( Game::Graphics::Polygon::LoadManyFromSVGNode( Resources::CursorPath.c_str(), node, scale )[ 0 ] );
    }

    Cursor::~Cursor()
    {
    }

    void Cursor::Render( sf::RenderTarget& target )
    {
        m_shape->Render( target );
    }

    void Cursor::SetPosition( const sf::Vector2f& value )
    {
        m_position = value;
        m_shape->SetPosition( m_position );
    }
}