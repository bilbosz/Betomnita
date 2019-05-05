#include "betomnita/gameplay/World.hpp"

#include "app/Debug.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/gameplay/DynamicObstacle.hpp"
#include "betomnita/gameplay/GamePlayLogic.hpp"
#include "betomnita/gameplay/PhysicalBody.hpp"
#include "betomnita/gameplay/Projectile.hpp"
#include "betomnita/gameplay/PrototypeDict.hpp"
#include "betomnita/gameplay/StaticObstacle.hpp"
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
#include <sstream>

#include "betomnita/gameplay/Vehicle.hpp"
#include "betomnita/gameplay/VehicleChassis.hpp"
#include "betomnita/gameplay/VehicleChassisPrototype.hpp"
#include "betomnita/gameplay/VehicleGunPrototype.hpp"

namespace Betomnita::GamePlay
{
    using Game::Utils::Cast;

    World::World( GamePlayLogic* logic ) : m_currentLogic( logic ), m_physicsWorld( b2Vec2( 0.0f, 0.0f ) )
    {
    }

    World::~World()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::RemoveListener( Resources::ListenerId::StartMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::RemoveListener( Resources::ListenerId::StopMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::RemoveListener( Resources::ListenerId::MoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::RemoveListener( Resources::ListenerId::Shot );
    }

    void World::Init()
    {
        InitView();
        InitPhysics();
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::AddListener( { Resources::ListenerId::Shot, false, [this]( const sf::Event::KeyEvent& key ) {
                                                                                        if( key.code == sf::Keyboard::Space )
                                                                                        {
                                                                                            m_vehicles[ 1 ].Shot();
                                                                                        }
                                                                                    } } );
    }

    void World::Render( sf::RenderTarget& target )
    {
        if( m_backgroundColor.has_value() )
        {
            target.clear( m_backgroundColor.value() );
        }
        auto transform = m_view.GetTransform();
        for( auto& terrain : m_terrainSheets )
        {
            terrain->Render( target, transform );
        }
        for( auto& staticObstacle : m_staticObstacles )
        {
            staticObstacle->Render( target, transform );
        }
        for( auto& dynamicObstacle : m_dynamicObstacles )
        {
            dynamicObstacle->Render( target, transform );
        }
        for( auto& vehicle : m_vehicles )
        {
            vehicle.second.Render( target, transform );
        }
        for( auto& projectile : m_projectiles )
        {
            projectile->Render( target, transform );
        }
    }

    void World::Update( const sf::Time& dt )
    {
        m_physicsWorld.Step( dt.asSeconds(), 8, 3 );
        for( b2Contact* contact = m_physicsWorld.GetContactList(); contact; contact = contact->GetNext() )
        {
            if( contact->IsTouching() )
            {
                for( auto& projectile : m_projectiles )
                {
                    if( projectile->GetPhysicalBody() == contact->GetFixtureA()->GetBody() || projectile->GetPhysicalBody() == contact->GetFixtureB()->GetBody() )
                    {
                        projectile->SetToDestroy();
                    }
                }
            }
        }
        UpdateView( dt );
        for( auto& dynamicObstacle : m_dynamicObstacles )
        {
            dynamicObstacle->Update( dt );
        }
        for( auto& vehicle : m_vehicles )
        {
            vehicle.second.Update( dt );
        }

        if( std::find_if( m_projectiles.begin(), m_projectiles.end(), []( auto& projectile ) { return projectile->IsSetToDestroy(); } ) != m_projectiles.end() )
        {
            m_projectiles.erase( std::remove_if( m_projectiles.begin(), m_projectiles.end(), []( auto& projectile ) { return projectile->IsSetToDestroy(); } ) );
        }
        for( auto& projectile : m_projectiles )
        {
            projectile->Update( dt );
        }
    }

    void World::Pause()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::DisableListener( Resources::ListenerId::StartMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::DisableListener( Resources::ListenerId::StopMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::DisableListener( Resources::ListenerId::MoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::DisableListener( Resources::ListenerId::Shot );
    }

    void World::Unpause()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::EnableListener( Resources::ListenerId::StartMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::EnableListener( Resources::ListenerId::StopMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::EnableListener( Resources::ListenerId::MoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::EnableListener( Resources::ListenerId::Shot );
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

        auto backgroundColorAttr = svgNode.attribute( "data-background-color" );
        if( backgroundColorAttr )
        {
            sf::Uint32 color;
            std::stringstream formatter;
            formatter << backgroundColorAttr.as_string();
            formatter >> std::hex >> color;
            m_backgroundColor = sf::Color( color );
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
                        auto& terrain = m_terrainSheets.emplace_back( std::make_unique< Terrain >( this ) );
                        terrain->LoadFromSVGNode( filename, def, node, scale );
                    }
                    else if( std::find( classes.begin(), classes.end(), "static-obstacle" ) != classes.end() )
                    {
                        auto& staticObstacle = m_staticObstacles.emplace_back( std::make_unique< StaticObstacle >( this ) );
                        staticObstacle->LoadFromSVGNode( filename, def, node, scale );
                    }
                    else if( std::find( classes.begin(), classes.end(), "dynamic-obstacle" ) != classes.end() )
                    {
                        auto& dynamicObstacle = m_dynamicObstacles.emplace_back( std::make_unique< DynamicObstacle >( this ) );
                        dynamicObstacle->LoadFromSVGNode( filename, def, node, scale );
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

                    auto it = m_vehicles.emplace( unitId, Vehicle{} );
                    auto& vehicle = it.first->second;
                    vehicle.SetId( unitId );
                    vehicle.World = this;
                    switch( prototype.GetType() )
                    {
                        case Prototype::Type::Chassis:
                            vehicle.Chassis.AssignVehicle( &vehicle );
                            vehicle.Chassis.LoadFromPrototype( prototype );
                            vehicle.Chassis.SetInitialPosition( transform.transformPoint( 0.0f, 0.0f ) );
                            break;
                        case Prototype::Type::Gun:
                            vehicle.Gun.AssignVehicle( &vehicle );
                            vehicle.Gun.LoadFromPrototype( prototype );
                            break;
                        default:
                            ASSERT( false, L"Wrong prototype type" );
                            break;
                    }
                }
                break;
            }
        }
        ASSERT( !m_vehicles.empty(), L"There should be at least one vehicle with id=1 for player to control" );
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

    void World::UpdateView( const sf::Time& dt )
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
        {
            b2MassData data;
            m_vehicles[ 1 ].Chassis.GetPhysicalBody()->GetMassData( &data );
            m_view.Center = Cast< sf::Vector2f >( m_vehicles[ 1 ].Chassis.GetPhysicalBody()->GetWorldPoint( data.center ) );
            m_view.Rotation = m_vehicles[ 1 ].Chassis.GetPhysicalBody()->GetAngle();
        }
    }

    void World::InitPhysics()
    {
        for( auto& staticObstacle : m_staticObstacles )
        {
            staticObstacle->InitPhysics();
        }
        for( auto& dynamicObstacle : m_dynamicObstacles )
        {
            dynamicObstacle->InitPhysics();
        }
        for( auto& vehicle : m_vehicles )
        {
            vehicle.second.InitPhysics();
        }
    }

    Projectile* World::AddProjectile( std::unique_ptr< Projectile > projectile )
    {
        m_projectiles.push_back( std::move( projectile ) );
        return m_projectiles.back().get();
    }
}