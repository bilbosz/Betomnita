#include "betomnita/gameplay/DynamicObstacle.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/World.hpp"
#include "game/GameConsts.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"
#include "game/utils/Utils.hpp"

#include <Box2D/Box2D.h>
#include <pugixml.hpp>

namespace Betomnita::GamePlay
{
    DynamicObstacle::DynamicObstacle( World* world ) : m_world( world )
    {
    }

    DynamicObstacle::~DynamicObstacle()
    {
    }

    void DynamicObstacle::Update( const sf::Time& dt )
    {
        m_transform = sf::Transform::Identity;
        m_transform.translate( Game::Utils::cast< sf::Vector2f >( m_physicalBody->GetPosition() ) );
        m_transform.rotate( m_physicalBody->GetAngle() * Game::Consts::RadToDeg );
    }

    void DynamicObstacle::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        sf::Transform r;
        r.combine( transform );
        r.combine( m_transform );
        for( auto& shape : m_shape )
        {
            shape.Render( target, r );
        }
    }

    void DynamicObstacle::LoadFromSVGNode( const std::string& filename, pugi::xml_document& doc, pugi::xml_node& node, float scale )
    {
        m_density = node.attribute( "data-density" ).as_float();
        ASSERT( m_density > 0.0f, L"Density should be a positive number" );

        m_shape = Game::Graphics::Polygon::LoadManyFromSVGNode( filename, doc, node, scale );

        m_aabb.Init( m_shape[ 0 ].GetAABB() );
        for( const auto& polygon : m_shape )
        {
            m_aabb.Constrain( polygon.GetAABB() );
        }
    }

    void DynamicObstacle::InitPhysics()
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.allowSleep = true;

        auto& physicsWorld = m_world->PhysicsWorld();
        m_physicalBody = physicsWorld.CreateBody( &bodyDef );

        b2PolygonShape shape;
        for( const auto& polygon : m_shape )
        {
            for( const auto& triangle : polygon.GetTriangulatedPoints() )
            {
                b2Vec2 points[ 3 ] = {
                    { triangle[ 0 ].x, triangle[ 0 ].y },
                    { triangle[ 1 ].x, triangle[ 1 ].y },
                    { triangle[ 2 ].x, triangle[ 2 ].y },
                };
                shape.Set( points, 3 );
                m_physicalBody->CreateFixture( &shape, m_density );
            }
        }
    }

    void DynamicObstacle::SetTransform( const sf::Transform& transform )
    {
        for( auto& shape : m_shape )
        {
            shape.Transform() = transform;
        }
    }
}