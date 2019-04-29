#include "betomnita/gameplay/StaticObstacle.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/World.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/SVGHelper.hpp"

#include <Box2D/Box2D.h>
#include <pugixml.hpp>

namespace Betomnita::GamePlay
{
    StaticObstacle::StaticObstacle( World* world ) : m_world( world )
    {
    }

    StaticObstacle::~StaticObstacle()
    {
    }

    void StaticObstacle::Update( const sf::Time& dt )
    {
    }

    void StaticObstacle::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        for( auto& shape : m_shape )
        {
            shape.Render( target, transform );
        }
    }

    void StaticObstacle::LoadFromSVGNode( const std::string& filename, pugi::xml_document& doc, pugi::xml_node& node, float scale )
    {
        m_shape = Game::Graphics::Polygon::LoadManyFromSVGNode( filename, doc, node, scale );

        m_aabb.Init( m_shape[ 0 ].GetAABB() );
        for( const auto& polygon : m_shape )
        {
            m_aabb.Constrain( polygon.GetAABB() );
        }
    }

    void StaticObstacle::InitPhysics()
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
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
                m_physicalBody->CreateFixture( &shape, 0.0f );
            }
        }
    }

    void StaticObstacle::SetTransform( const sf::Transform& transform )
    {
        for( auto& shape : m_shape )
        {
            shape.Transform() = transform;
        }
    }
}