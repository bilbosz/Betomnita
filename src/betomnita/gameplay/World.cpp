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
            terrain->GetShape()->Render( target );
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
            scale = scaleAttr.as_float( 1.0f );
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
#ifdef DEBUG
                    auto id = node.attribute( "id" ).as_string();
#endif
                    auto desc = Game::Graphics::SVGHelper::ParsePathDescriptions( node.attribute( "d" ).as_string() );
                    auto style = Game::Graphics::SVGHelper::ParseStyle( node.attribute( "style" ).as_string() );
                    auto transform = Game::Graphics::SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() );
                    auto classes = Game::Graphics::SVGHelper::ParseClass( node.attribute( "class" ).as_string() );

                    auto current = &node.parent();
                    while( *current->name() == '\0' )
                    {
                        transform.combine( Game::Graphics::SVGHelper::ParseTransform( current->attribute( "transform" ).as_string() ) );
                        current = &current->parent();
                    }

                    for( auto& points : desc )
                    {
                        auto polygon = std::make_unique< Game::Graphics::Polygon >();
                        for( auto& point : points )
                        {
                            point = transform.transformPoint( point );
                        }
                        while( !points.empty() && *points.cbegin() == *points.crbegin() )
                        {
                            points.pop_back();
                        }
#ifdef DEBUG
                        if( App::Debug::IsExpensive() )
                        {
                            auto error = Game::Graphics::Polygon::GetPointsErrors( points );
                            switch( error )
                            {
                                case Game::Graphics::Polygon::Error::PointsNotUnique:
                                    ASSERT( false, L"Polygon \"" << id << L"\" in file " << filename.c_str() << L" have not unique points." );
                                    break;
                                case Game::Graphics::Polygon::Error::LinesCrossing:
                                    ASSERT( false, L"Polygon \"" << id << L"\" in file " << filename.c_str() << L" have lines crossing." );
                                    break;
                            }
                        }
#endif
                        if( !Game::Graphics::Polygon::IsRightDirection( points ) )
                        {
                            WARNING( L"Verticies for polygon \"" << id << L"\" in file " << filename.c_str() << L" have to be reversed." );
                            std::reverse( points.begin(), points.end() );
                        }
                        polygon->SetPoints( points );

                        auto fillColorStyle = style.find( "fill" );
                        uint32_t fillColor = 0x00000000;
                        if( fillColorStyle != style.cend() )
                        {
                            std::istringstream( fillColorStyle->second.substr( 1 ) ) >> std::hex >> fillColor;
                            fillColor = fillColor << 8 | 0xff;
                        }

                        const auto& opacityStyle = style.find( "opacity" );
                        float opacity = 1.0f;
                        if( opacityStyle != style.cend() )
                        {
                            opacity = static_cast< float >( atof( opacityStyle->second.c_str() ) );
                            fillColor &= UINT_MAX << 8 | static_cast< uint32_t >( opacity * 255.0f );
                        }
                        polygon->SetColor( sf::Color( fillColor ) );

                        for( const auto& class_ : classes )
                        {
                            if( class_ == "terrain" )
                            {
                                m_terrainSheets.emplace_back( std::make_unique< Terrain >() );
                                m_terrainSheets.back()->SetShape( std::move( polygon ) );
                                m_terrainSheets.back()->SetFriction( node.attribute( "data-friction" ).as_float() );
                            }
                        }
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