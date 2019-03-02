#include "betomnita/gameplay/VehicleChassisPrototype.hpp"

#include "app/Debug.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"

#include <pugixml.hpp>

namespace Betomnita::GamePlay
{
    VehicleChassisPrototype::VehicleChassisPrototype() : Prototype( Type::Chassis ), m_shape()
    {
    }

    void VehicleChassisPrototype::LoadFromFile( const std::string& filename )
    {
        m_shape.clear();

        pugi::xml_document doc;
        VERIFY( doc.load_file( filename.c_str() ) );

        auto svgNode = doc.child( "svg" );
        auto scaleAttr = svgNode.attribute( "data-scale" );

        auto scale = 1.0f;
        if( scaleAttr )
        {
            scale = scaleAttr.as_float( scale );
        }

        auto& elems = svgNode.select_nodes( "//path | //ellipse" );

        for( auto& elem : elems )
        {
            auto& node = elem.node();
            const auto nodeName = node.name();
            auto classes = Game::Graphics::SVGHelper::ParseClass( node.attribute( "class" ).as_string() );
            CHECK( strcmp( nodeName, "path" ) == 0 || strcmp( nodeName, "ellipse" ) == 0 );
            switch( *nodeName )
            {
                case 'p':
                {
                    if( std::find( classes.begin(), classes.end(), "physical-body-shape" ) != classes.end() )
                    {
                        auto pathDesc = Game::Graphics::SVGHelper::ParsePathDescriptions( node.attribute( "d" ).as_string() );
                        ASSERT( pathDesc.size() == 1, L"Physical body shape should be in one piece" );

                        m_physicalBodyShape = pathDesc[ 0 ];
                        sf::Transform transform;
                        transform.scale( { scale, scale } );
                        transform.combine( Game::Graphics::SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() ) );

                        auto current = &node.parent();
                        while( *current->name() == '\0' )
                        {
                            transform.combine( Game::Graphics::SVGHelper::ParseTransform( current->attribute( "transform" ).as_string() ) );
                            current = &current->parent();
                        }
                        for( auto& point : m_physicalBodyShape )
                        {
                            point = transform.transformPoint( point );
                        }
                    }
                    else
                    {
                        auto polygons = Game::Graphics::Polygon::LoadManyFromSVGNode( filename, doc, node, scale );
                        std::move( polygons.cbegin(), polygons.cend(), std::back_inserter( m_shape ) );
                    }
                }
                break;
                case 'e':
                {
                    sf::Vector2f point;
                    point.x = node.attribute( "cx" ).as_float();
                    point.y = node.attribute( "cy" ).as_float();

                    sf::Transform transform;
                    transform.scale( { scale, scale } );
                    transform.combine( Game::Graphics::SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() ) );

                    auto current = &node.parent();
                    while( *current->name() == '\0' )
                    {
                        transform.combine( Game::Graphics::SVGHelper::ParseTransform( current->attribute( "transform" ).as_string() ) );
                        current = &current->parent();
                    }
                    point = transform.transformPoint( point );

                    if( std::find( classes.begin(), classes.end(), "pivot" ) != classes.end() )
                    {
                        m_pivot = point;
                    }
                    if( std::find( classes.begin(), classes.end(), "gun-rotator-slot" ) != classes.end() )
                    {
                        m_gunRotatorSlot = point;
                    }
                }
                break;
            }
        }
        for( auto& polygon : m_shape )
        {
            polygon.SetPivot( m_pivot );
        }
    }
}