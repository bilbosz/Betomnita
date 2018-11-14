#include "Polygon.hpp"

#include "game/GenericGame.hpp"
#include "resource/Resource.hpp"

namespace Graphics
{
    Polygon::Polygon() : m_vertexArray( sf::PrimitiveType::Triangles )
    {
    }

    Polygon::~Polygon()
    {
    }

    void Polygon::Render( sf::RenderTarget& target )
    {
        if( m_vertexArray.getVertexCount() >= 3 )
        {
            target.draw( m_vertexArray, Game::GenericGame::GetInstance()->GetToScreenTransform() );
        }
    }

    void Polygon::SetPoints( const PointsVector& value )
    {
        ASSERT( value.size() >= 3, L"Polygon has to have at least 3 verticies" );
        m_points = value;
        Triangulate();
    }

    void Polygon::Init()
    {
        Primitive::Init();
    }

    void Polygon::OnPositionChange( const sf::Vector2f& newPosition )
    {
    }

    void Polygon::OnSizeChange( const sf::Vector2f& newSize )
    {
    }

    void Polygon::Triangulate()
    {
        auto polygonVerticies = PointsList( m_points.cbegin(), m_points.cend() );
        auto adv = [&polygonVerticies]( PointsListIter& iter ) {
            std::advance( iter, 1 );
            if( iter == polygonVerticies.cend() )
            {
                iter = polygonVerticies.cbegin();
            }
        };

        m_triangles.clear();
        m_vertexArray.clear();
        auto prev = polygonVerticies.cbegin();
        auto curr = std::next( prev );
        auto next = std::next( curr );
        for( ; polygonVerticies.size() != 2; )
        {
            if( IsEar( polygonVerticies, prev, curr, next ) )
            {
                m_triangles.emplace_back( *prev, *curr, *next );
                polygonVerticies.erase( curr );
                curr = next;
                adv( next );
            }
            else
            {
                prev = curr;
                curr = next;
                adv( next );
            }
        }

        for( const auto& triangle : m_triangles )
        {
            m_vertexArray.append( sf::Vertex( std::get< 0 >( triangle ), sf::Color::Red ) );
            m_vertexArray.append( sf::Vertex( std::get< 1 >( triangle ), sf::Color::Green ) );
            m_vertexArray.append( sf::Vertex( std::get< 2 >( triangle ), sf::Color::Blue ) );
        }
    }

    bool Polygon::IsEar( const PointsList& polygonVerticies, PointsListIter previousVertex, PointsListIter currentVertex, PointsListIter nextVertex ) const
    {
        auto angle = GetAngle( *previousVertex, *currentVertex, *nextVertex );
        if( angle >= Resource::Pi )
        {
            return false;
        }

        auto it = polygonVerticies.cbegin();
        for( ; it != polygonVerticies.cend(); ++it )
        {
            if( it == previousVertex || it == currentVertex || it == nextVertex )
            {
                continue;
            }
            if( IsPointInsideTriangle( *it, *previousVertex, *currentVertex, *nextVertex ) )
            {
                return false;
            }
        }
        return true;
    }

    float Polygon::GetAngle( const Point& previousVertex, const Point& currentVertex, const Point& nextVertex ) const
    {
        auto prev = previousVertex - currentVertex;
        auto next = nextVertex - currentVertex;
        return fmodf( atan2f( prev.y, prev.x ) - atan2f( next.y, next.x ) + 2.0f * Resource::Pi, 2.0f * Resource::Pi );
    }

    bool Polygon::IsPointInsideTriangle( const Point& examinedPoint, const Point& a, const Point& b, const Point& c ) const
    {
        return GetAngle( examinedPoint, a, b ) < Resource::Pi && GetAngle( examinedPoint, b, c ) < Resource::Pi && GetAngle( examinedPoint, c, a ) < Resource::Pi;
    }
}
