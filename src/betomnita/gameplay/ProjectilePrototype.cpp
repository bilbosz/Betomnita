#include "betomnita/gameplay/ProjectilePrototype.hpp"

#include "app/Debug.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"

#include <pugixml.hpp>

namespace Betomnita::GamePlay
{
    ProjectilePrototype::ProjectilePrototype() : Prototype( Type::Projectile ), m_shape(), m_density()
    {
    }

    void ProjectilePrototype::LoadFromFile( const std::string& filename )
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

        auto elems = svgNode.select_nodes( "//path" );

        for( auto& elem : elems )
        {
            auto node = elem.node();
            const auto nodeName = node.name();
            auto classes = Game::Graphics::SVGHelper::ParseClass( node.attribute( "class" ).as_string() );
            CHECK( strcmp( nodeName, "path" ) == 0 );
            switch( *nodeName )
            {
                case 'p':
                {
                    if( std::find( classes.begin(), classes.end(), "shot-direction" ) != classes.end() )
                    {
                        auto pathDesc = Game::Graphics::SVGHelper::ParsePathDescriptions( node.attribute( "d" ).as_string() );
                        ASSERT( pathDesc.size() == 1 && pathDesc[ 0 ].size() == 2, L"Shot direction should have only 2 points" );

                        m_direction.Source = pathDesc[ 0 ][ 0 ];
                        m_direction.Destination = pathDesc[ 0 ][ 1 ];
                        sf::Transform transform;
                        transform.scale( { scale, scale } );
                        transform.combine( Game::Graphics::SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() ) );

                        auto current = node.parent();
                        while( *current.name() == '\0' )
                        {
                            transform.combine( Game::Graphics::SVGHelper::ParseTransform( current.attribute( "transform" ).as_string() ) );
                            current = current.parent();
                        }
                        m_direction.Source = transform.transformPoint( m_direction.Source );
                        m_direction.Destination = transform.transformPoint( m_direction.Destination );
                    }
                    else if( std::find( classes.begin(), classes.end(), "physical-body-shape" ) != classes.end() )
                    {
                        auto polygons = Game::Graphics::Polygon::LoadManyFromSVGNode( filename, doc, node, scale );
                        for( auto& polygon : polygons )
                        {
                            const auto& triangulatedPoints = polygon.GetTriangulatedPoints();
                            for( const auto& triangle : triangulatedPoints )
                            {
                                m_physicalBodyShape.emplace_back( triangle );
                            }
                        }
                        m_density = node.attribute( "data-density" ).as_float();
                        ASSERT( m_density > 0.0f, L"Density has to be greater than 0" );
                    }
                    else
                    {
                        auto polygons = Game::Graphics::Polygon::LoadManyFromSVGNode( filename, doc, node, scale );
                        std::move( polygons.cbegin(), polygons.cend(), std::back_inserter( m_shape ) );
                    }
                }
                break;
            }
        }
    }
}