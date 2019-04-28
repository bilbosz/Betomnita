#include "betomnita/gameplay/World.hpp"

#include "app/Debug.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/gameplay/GamePlayLogic.hpp"
#include "betomnita/gameplay/PhysicalBody.hpp"
#include "betomnita/gameplay/PrototypeDict.hpp"
#include "betomnita/gameplay/Terrain.hpp"
#include "betomnita/resources/Resources.hpp"
#include "game/GenericGame.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"
#include "game/utils/Utils.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <map>
#include <pugixml.hpp>

#include "betomnita/gameplay/Vehicle.hpp"
#include "betomnita/gameplay/VehicleChassis.hpp"
#include "betomnita/gameplay/VehicleChassisPrototype.hpp"
#include "betomnita/gameplay/VehicleGunPrototype.hpp"

namespace Betomnita::GamePlay
{
    using Game::Utils::cast;

    World::World( GamePlayLogic* logic ) : m_currentLogic( logic ), m_physicsWorld( b2Vec2( 0.0f, 0.0f ) )
    {
    }

    World::~World()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::RemoveListener( Resources::ListenerId::StartMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::RemoveListener( Resources::ListenerId::StopMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::RemoveListener( Resources::ListenerId::MoveWorld );
    }

    void World::Init()
    {
        InitView();
        InitPhysics();
    }

    void World::Render( sf::RenderTarget& target )
    {
        auto transform = m_view.GetTransform();
        for( auto& terrain : m_terrainSheets )
        {
            terrain->Render( target, transform );
        }
        for( auto& vehicle : m_vehicles )
        {
            vehicle.Render( target, transform );
        }
    }

    void World::Update( const sf::Time& dt )
    {
        {
            auto newScale = m_view.Scale;
            auto scaleChanged = false;
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Equal ) )
            {
                scaleChanged = true;
                newScale *= powf( Resources::ZoomFactor, dt.asSeconds() );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Hyphen ) )
            {
                scaleChanged = true;
                newScale /= powf( Resources::ZoomFactor, dt.asSeconds() );
            }
            if( scaleChanged && newScale >= Resources::ZoomOutLimit && newScale <= Resources::ZoomInLimit )
            {
                m_view.Scale = newScale;
            }
        }
        m_physicsWorld.Step( dt.asSeconds(), 8, 3 );
        for( auto& vehicle : m_vehicles )
        {
            vehicle.Update( dt );
            auto physicalBody = vehicle.Chassis().GetPhysicalBody();
            float impulse = 370'000.0f * dt.asSeconds();
            auto angle = physicalBody->GetAngle() + Game::Consts::Pi * 0.5f;
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
            {
                physicalBody->ApplyLinearImpulseToCenter( b2Vec2( -impulse * cosf( angle ), -impulse * sinf( angle ) ), true );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
            {
                physicalBody->ApplyLinearImpulseToCenter( b2Vec2( impulse * cosf( angle ), impulse * sinf( angle ) ), true );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
            {
                physicalBody->ApplyAngularImpulse( -impulse, true );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
            {
                physicalBody->ApplyAngularImpulse( impulse, true );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Num0 ) )
            {
                physicalBody->SetAngularVelocity( 0.0f );
                physicalBody->SetLinearVelocity( b2Vec2( 0.0f, 0.0f ) );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) )
            {
                vehicle.Gun().SetDirection( vehicle.Gun().GetDirection() - 1.0f * dt.asSeconds() );
            }
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) )
            {
                vehicle.Gun().SetDirection( vehicle.Gun().GetDirection() + 1.0f * dt.asSeconds() );
            }
            b2MassData data;
            vehicle.Chassis().GetPhysicalBody()->GetMassData( &data );
            m_view.Center = cast< sf::Vector2f >( vehicle.Chassis().GetPhysicalBody()->GetWorldPoint( data.center ) );
            m_view.Rotation = vehicle.Chassis().GetPhysicalBody()->GetAngle();
        }
    }

    void World::Pause()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::GetListener( Resources::ListenerId::StartMoveWorld ).IsEnabled = false;
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::GetListener( Resources::ListenerId::StopMoveWorld ).IsEnabled = false;
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::GetListener( Resources::ListenerId::MoveWorld ).IsEnabled = false;
    }

    void World::Unpause()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::GetListener( Resources::ListenerId::StartMoveWorld ).IsEnabled = true;
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::GetListener( Resources::ListenerId::StopMoveWorld ).IsEnabled = true;
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::GetListener( Resources::ListenerId::MoveWorld ).IsEnabled = true;
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

        auto physicalScale = 1.0f;
        auto physicalScaleAttr = svgNode.attribute( "data-physical-scale" );
        if( physicalScaleAttr )
        {
            physicalScale = physicalScaleAttr.as_float( physicalScale );
        }

        std::map< int, sf::Vector2f > vehiclesPositions;

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
                        auto& terrain = m_terrainSheets.emplace_back( std::make_unique< Terrain >( this ) );
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

                    sf::Transform transform;
                    transform.scale( scale, scale );
                    transform.combine( Game::Graphics::SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() ) );

                    auto unitId = node.attribute( "data-unit" ).as_int();

                    const auto& prototype = m_currentLogic->GetPrototypeDict().GetPrototypeByName( normalizedHref );

                    auto it = std::lower_bound( m_vehicles.begin(), m_vehicles.end(), unitId, []( const Vehicle& vehicle, int id ) { return vehicle.GetId() < id; } );
                    if( it == m_vehicles.end() || it->GetId() != unitId )
                    {
                        it = m_vehicles.emplace( it, this );
                        it->SetId( unitId );
                    }
                    switch( prototype.GetType() )
                    {
                        case Prototype::Type::Chassis:
                            it->Chassis().AssignVehicle( &*it );
                            it->Chassis().LoadFromPrototype( prototype );
                            vehiclesPositions[ unitId ] = transform.transformPoint( 0.0f, 0.0f );
                            break;
                        case Prototype::Type::Gun:
                            it->Gun().AssignVehicle( &*it );
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
        for( auto& vehicle : m_vehicles )
        {
            vehicle.Chassis().SetInitialPosition( vehiclesPositions[ vehicle.GetId() ] );
        }
        m_size.Init( m_terrainSheets[ 0 ]->GetAABB() );
        for( const auto& terrain : m_terrainSheets )
        {
            m_size.Constrain( terrain->GetAABB() );
        }
    }

    void World::InitView()
    {
        m_view.Scale = Resources::ZoomDefault;
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::AddListener(
            { Resources::ListenerId::StartMoveWorld, false, [this]( const sf::Vector2f& pos, sf::Mouse::Button btn ) {
                 if( btn == sf::Mouse::Button::Right )
                 {
                     m_moving = true;
                     m_previousPoint = pos;
                 }
             } } );
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::AddListener(
            { Resources::ListenerId::StopMoveWorld, false, [this]( const sf::Vector2f& pos, sf::Mouse::Button btn ) {
                 if( btn == sf::Mouse::Button::Right )
                 {
                     m_moving = false;
                 }
             } } );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::AddListener( { Resources::ListenerId::MoveWorld, false, [this]( const sf::Vector2f& pos ) {
                                                                                        if( m_moving )
                                                                                        {
                                                                                            sf::Vector2f diff = ( pos - m_previousPoint ) / m_view.Scale;
                                                                                            m_previousPoint = pos;
                                                                                            auto newCenter = m_view.Center - diff;
                                                                                            if( newCenter.x < m_size.MinX || newCenter.x > m_size.MaxX )
                                                                                            {
                                                                                                diff.x = 0.0f;
                                                                                            }
                                                                                            if( newCenter.y < m_size.MinY || newCenter.y > m_size.MaxY )
                                                                                            {
                                                                                                diff.y = 0.0f;
                                                                                            }
                                                                                            m_view.Center -= diff;
                                                                                        }
                                                                                    } } );
    }

    void World::InitPhysics()
    {
        for( auto& vehicle : m_vehicles )
        {
            vehicle.InitPhysics();
        }
    }
}