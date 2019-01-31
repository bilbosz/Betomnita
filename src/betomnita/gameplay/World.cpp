#include "betomnita/gameplay/World.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/PhysicalBody.hpp"
#include "betomnita/gameplay/Terrain.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"

#include "pugixml.hpp"
#include <algorithm>
#include <cstring>
#include <filesystem>

namespace Betomnita::GamePlay
{
    World::World()
    {
    }

    World::~World()
    {
    }

    void World::Render( sf::RenderTarget& target )
    {
        for( auto& terrain : m_terrainSheets )
        {
            terrain->Render( target );
        }
    }

    void World::Update( const sf::Time& dt )
    {
    }

    void World::LoadFromFile( const std::string& filename )
    {
        pugi::xml_document def;
        VERIFY( def.load_file( filename.c_str() ) );

        auto svgNode = def.child( "svg" );
        auto scaleAttr = svgNode.attribute( "data-scale" );

        auto scale = 1.0f;
        if( scaleAttr )
        {
            scale = scaleAttr.as_float( scale );
        }

        const auto& elems = svgNode.select_nodes( "//path | //image" );

        for( const auto& elem : elems )
        {
            const auto& node = elem.node();
            const auto nodeName = node.name();
            CHECK( strcmp( nodeName, "path" ) == 0 || strcmp( nodeName, "image" ) == 0 );
            switch( *nodeName )
            {
                case 'p':
                {
                    auto classes = Game::Graphics::SVGHelper::ParseClass( node.attribute( "class" ).as_string() );
                    if( std::find( classes.begin(), classes.end(), "terrain" ) != classes.end() )
                    {
                        auto& terrain = m_terrainSheets.emplace_back( std::make_unique< Terrain >() );
                        terrain->LoadFromSVGNode( filename, node, scale );
                    }
                }
                break;
                case 'i':
                {
#ifdef DEBUG
                    auto id = node.attribute( "id" ).as_string();
#endif
                    auto href = node.attribute( "href" ).as_string();
                    if( *href == '\0' )
                    {
                        href = node.attribute( "xlink:href" ).as_string();
                    }
                    auto normalizedHref = ( std::filesystem::path( filename ).parent_path() / href ).lexically_normal().wstring();

                    auto transform = Game::Graphics::SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() );
                }
                break;
            }
        }
    }
}