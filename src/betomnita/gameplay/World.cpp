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
    World::World( GamePlayLogic* logic ) : m_currentLogic( logic ), m_physicsWorld( b2Vec2( 0.0f, 0.0f ) )
    {
    }

    World::~World()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseWheelScrolled >::RemoveListener( Resources::ListenerId::ZoomInOutWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::RemoveListener( Resources::ListenerId::StartMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::RemoveListener( Resources::ListenerId::StopMoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::RemoveListener( Resources::ListenerId::MoveWorld );
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::RemoveListener( Resources::ListenerId::TankForward );
    }

    void World::Init()
    {
        InitView();
        InitPhysics();
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
            auto physicalBody = vehicle.GetPhysicalBody();
            physicalBody->ApplyForce( m_acceleration, physicalBody->GetWorldCenter(), true );
        }
        m_physicsWorld.Step( dt.asSeconds(), 8, 3 );
        for( auto& vehicle : m_vehicles )
        {
            auto newPosition = vehicle.GetPhysicalBody()->GetPosition();
            vehicle.SetPosition( { newPosition.x, newPosition.y } );
            vehicle.Update( dt );
        }
    }

    void World::Pause()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseWheelScrolled >::GetListener( Resources::ListenerId::ZoomInOutWorld ).IsEnabled = false;
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::GetListener( Resources::ListenerId::StartMoveWorld ).IsEnabled = false;
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::GetListener( Resources::ListenerId::StopMoveWorld ).IsEnabled = false;
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::GetListener( Resources::ListenerId::MoveWorld ).IsEnabled = false;
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::GetListener( Resources::ListenerId::TankForward ).IsEnabled = false;
    }

    void World::Unpause()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseWheelScrolled >::GetListener( Resources::ListenerId::ZoomInOutWorld ).IsEnabled = true;
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::GetListener( Resources::ListenerId::StartMoveWorld ).IsEnabled = true;
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonReleased >::GetListener( Resources::ListenerId::StopMoveWorld ).IsEnabled = true;
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::GetListener( Resources::ListenerId::MoveWorld ).IsEnabled = true;
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::GetListener( Resources::ListenerId::TankForward ).IsEnabled = true;
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
                            it->Chassis().LoadFromPrototype( prototype );
                            vehiclesPositions[ unitId ] = transform.transformPoint( 0.0f, 0.0f );
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
        for( auto& vehicle : m_vehicles )
        {
            vehicle.SetPosition( vehiclesPositions[ vehicle.GetId() ] + vehicle.Chassis().GetPivot() );
        }
        m_size.Init( m_terrainSheets[ 0 ]->GetAABB() );
        for( const auto& terrain : m_terrainSheets )
        {
            m_size.Constrain( terrain->GetAABB() );
        }
    }

    void World::InitView()
    {
        m_view.scale( { Resources::ZoomDefault, Resources::ZoomDefault } );
        UpdateView();
        Game::EventSystem::Event< Resources::EventId::OnMouseWheelScrolled >::AddListener(
            { Resources::ListenerId::ZoomInOutWorld, false, [this]( float delta ) {
                 float zoom;
                 if( delta >= 0.0f )
                 {
                     zoom = delta * Resources::ZoomFactor;
                 }
                 else
                 {
                     zoom = -delta / Resources::ZoomFactor;
                 }

                 auto scale = GetViewScale() * zoom;
                 if( scale > Resources::ZoomInLimit || scale < Resources::ZoomOutLimit )
                 {
                     return;
                 }

                 m_view.scale( { zoom, zoom }, m_view.getInverse().transformPoint( Game::GenericGame::GetInstance()->GetMousePosition() ) );
                 UpdateView();
             } } );

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
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::AddListener(
            { Resources::ListenerId::MoveWorld, false, [this]( const sf::Vector2f& pos ) {
                 if( m_moving )
                 {
                     float scale = GetViewScale();
                     auto center = GetViewCenter();
                     sf::Vector2f diff = ( m_previousPoint - pos ) / scale;
                     if( center.x < m_size.MinX && diff.x < 0.0f || center.x > m_size.MaxX && diff.x > 0.0f )
                     {
                         diff.x = 0.0f;
                     }
                     if( center.y < m_size.MinY && diff.y < 0.0f || center.y > m_size.MaxY && diff.y > 0.0f )
                     {
                         diff.y = 0.0f;
                     }
                     m_previousPoint = pos;
                     m_view.translate( -diff );
                     UpdateView();
                 }
             } } );
    }

    void World::UpdateView()
    {
        for( auto& terrain : m_terrainSheets )
        {
            terrain->SetTransform( m_view );
        }
        for( auto& vehicle : m_vehicles )
        {
            vehicle.SetTransform( m_view );
        }
    }

    sf::Vector2f World::GetViewCenter() const
    {
        const auto& matrix = m_view.getMatrix();
        return { ( -matrix[ 12 ] + 0.5f ) / matrix[ 0 ], ( -matrix[ 13 ] + 0.5f ) / matrix[ 0 ] };
    }

    float World::GetViewScale() const
    {
        return m_view.getMatrix()[ 0 ];
    }

    void World::InitPhysics()
    {
        for( auto& vehicle : m_vehicles )
        {
            const sf::Vector2f position = vehicle.GetPosition();
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set( position.x, position.y );

            b2Body* body = m_physicsWorld.CreateBody( &bodyDef );
            vehicle.SetPhysicalBody( body );
            b2PolygonShape shape;
            for( const auto& triangle : Game::Graphics::Polygon::Triangulate( vehicle.Chassis().GetPhysicalBodyShape() ) )
            {
                b2Vec2 points[ 3 ] = {
                    { triangle[ 0 ].x, triangle[ 0 ].y },
                    { triangle[ 1 ].x, triangle[ 1 ].y },
                    { triangle[ 2 ].x, triangle[ 2 ].y },
                };
                shape.Set( points, 3 );
                body->CreateFixture( &shape, vehicle.Chassis().GetDensity() );
            }
            float mass = body->GetMass();
            MESSAGE( L"Mass is equal " << mass << L" kg" );
        }
        m_acceleration.SetZero();
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::AddListener( { Resources::ListenerId::TankForward, true, [this]( const sf::Event::KeyEvent& key ) {
                                                                                        switch( key.code )
                                                                                        {
                                                                                            case sf::Keyboard::Key::W:
                                                                                                m_acceleration.y = -370000.0f / 60.0f;
                                                                                                break;
                                                                                            case sf::Keyboard::Key::S:
                                                                                                m_acceleration.y = 370000.0f / 60.0f;
                                                                                                break;
                                                                                            case sf::Keyboard::Key::A:
                                                                                                m_acceleration.x = -370000.0f;
                                                                                                break;
                                                                                            case sf::Keyboard::Key::D:
                                                                                                m_acceleration.x = 370000.0f;
                                                                                                break;
                                                                                            case sf::Keyboard::Key::Num0:
                                                                                                m_acceleration.SetZero();
                                                                                                break;
                                                                                        }
                                                                                    } } );
    }
}