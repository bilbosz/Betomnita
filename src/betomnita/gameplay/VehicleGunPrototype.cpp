#include "betomnita/gameplay/VehicleGunPrototype.hpp"

#include "app/Debug.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"

#include <pugixml.hpp>

namespace Betomnita::GamePlay
{
    VehicleGunPrototype::VehicleGunPrototype() : Prototype( Type::Gun )
    {
    }

    void VehicleGunPrototype::LoadFromFile( const std::string& filename )
    {
        pugi::xml_document doc;
        VERIFY( doc.load_file( filename.c_str() ) );

        auto svgNode = doc.child( "svg" );
        auto scaleAttr = svgNode.attribute( "data-scale" );

        auto scale = 1.0f;
        if( scaleAttr )
        {
            scale = scaleAttr.as_float( scale );
        }

        const auto& elems = svgNode.select_nodes( "//path | //ellipse" );

        for( const auto& elem : elems )
        {
            const auto& node = elem.node();
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
                    else if( std::find( classes.begin(), classes.end(), "shot-direction" ) != classes.end() )
                    {
                        auto pathDesc = Game::Graphics::SVGHelper::ParsePathDescriptions( node.attribute( "d" ).as_string() );
                        ASSERT( pathDesc.size() == 1 && pathDesc[ 0 ].size() == 2, L"Shot direction should have only 2 points" );

                        m_shotDirection.Source = pathDesc[ 0 ][ 0 ];
                        m_shotDirection.Destination = pathDesc[ 0 ][ 1 ];
                        sf::Transform transform;
                        transform.scale( { scale, scale } );
                        transform.combine( Game::Graphics::SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() ) );

                        auto current = &node.parent();
                        while( *current->name() == '\0' )
                        {
                            transform.combine( Game::Graphics::SVGHelper::ParseTransform( current->attribute( "transform" ).as_string() ) );
                            current = &current->parent();
                        }
                        m_shotDirection.Source = transform.transformPoint( m_shotDirection.Source );
                        m_shotDirection.Destination = transform.transformPoint( m_shotDirection.Destination );
                    }
                    else
                    {
                        auto shape = Game::Graphics::Polygon::LoadManyFromSVGNode( filename, node, scale );
                        std::move( shape.begin(), shape.end(), std::back_inserter( m_shape ) );
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

                    if( std::find( classes.begin(), classes.end(), "gun-rotator-slot" ) != classes.end() )
                    {
                        m_gunRotator = point;
                    }
                }
                break;
            }
        }
    }
}