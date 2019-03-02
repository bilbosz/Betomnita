#include "betomnita/gameplay/World.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/GamePlayLogic.hpp"
#include "betomnita/gameplay/PhysicalBody.hpp"
#include "betomnita/gameplay/PrototypeDict.hpp"
#include "betomnita/gameplay/Terrain.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <pugixml.hpp>

#include "betomnita/gameplay/Vehicle.hpp"
#include "betomnita/gameplay/VehicleChassis.hpp"
#include "betomnita/gameplay/VehicleChassisPrototype.hpp"
#include "betomnita/gameplay/VehicleGunPrototype.hpp"

namespace Betomnita::GamePlay
{
    World::World( GamePlayLogic* logic ) : m_currentLogic( logic )
    {
    }

    World::~World()
    {
    }

    void World::Init()
    {
    }

    void World::Render( sf::RenderTarget& target )
    {
        for( auto& terrain : m_terrainSheets )
        {
            terrain->Render( target );
        }
        for( auto& vehicle : m_vehicles )
        {
            vehicle.Render( target );
        }
    }

    void World::Update( const sf::Time& dt )
    {
        for( auto& vehicle : m_vehicles )
        {
            vehicle.Update( dt );
        }
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

        auto& elems = svgNode.select_nodes( "//path | //image" );

        for( auto& elem : elems )
        {
            auto& node = elem.node();
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
                        terrain->LoadFromSVGNode( filename, def, node, scale );
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
                    auto normalizedHref = ( std::filesystem::path( filename ).parent_path() / href ).lexically_normal().generic_string();

                    auto transform = Game::Graphics::SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() );

                    auto unitId = node.attribute( "data-unit" ).as_int();

                    const auto& prototype = m_currentLogic->GetPrototypeDict().GetPrototypeByName( normalizedHref );

                    auto it = std::lower_bound( m_vehicles.begin(), m_vehicles.end(), unitId, []( const Vehicle& vehicle, int id ) { return vehicle.GetId() < id; } );
                    if( it == m_vehicles.end() || it->GetId() != unitId )
                    {
                        it = m_vehicles.emplace( it );
                        it->SetId( unitId );
                    }
                    switch( prototype.GetType() )
                    {
                        case Prototype::Type::Chassis:
                            it->Chassis().LoadFromPrototype( prototype );
                            break;
                        case Prototype::Type::Gun:
                            it->Gun().LoadFromPrototype( prototype );
                            break;
                        default:
                            ASSERT( false, L"Wrong prototype type" );
                            break;
                    }
                }
                break;
            }
        }
    }
}