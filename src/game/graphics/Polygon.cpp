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
#ifdef DEBUG
        if( App::Debug::IsExpensive() )
        {
            auto error = GetPointsErrors( value );
            ASSERT( !error.has_value(), error.value() );
        }
#endif
        m_points = value;
        OnPointsChange( m_points );
        Triangulate();
    }

    std::optional< std::wstring > Polygon::GetPointsErrors( const PointsVector& points )
    {
        auto pointsN = points.size();
        if( pointsN == 0 )
        {
            return std::nullopt;
        }
        if( pointsN < 3 )
        {
            return L"Polygon has to have at least 3 vertices";
        }

        for( auto i = 0; i < pointsN; ++i )
        {
            for( auto j = i + 1; j < pointsN; ++j )
            {
                if( points[ i ] == points[ j ] )
                {
                    return L"Points in path defining polygon have to be unique";
                }
            }
        }

        for( auto i = 0; i < pointsN; ++i )
        {
            std::pair< const Point&, const Point& > lineA = { points[ i ], points[ ( i + 1 ) % pointsN ] };
            for( auto j = 0; j < pointsN; ++j )
            {
                if( j == i || j == ( i + 1 ) % pointsN || ( j + 1 ) % pointsN == i || ( j + 1 ) % pointsN == ( i + 1 ) % pointsN )
                {
                    continue;
                }
                std::pair< const Point&, const Point& > lineB = { points[ j ], points[ ( j + 1 ) % pointsN ] };
                if( AreLinesOverlapped( lineA, lineB ) )
                {
                    return L"Lines on perimeter cannot cross with itself in polygon";
                }
            }
        }

        auto angleSum = 0.0f;
        for( auto i = 0; i < pointsN; ++i )
        {
            angleSum += GetAngle( points[ i ], points[ ( i + 1 ) % pointsN ], points[ ( i + 2 ) % pointsN ] );
        }
        if( angleSum >= pointsN * Resource::Pi )
        {
            return L"Points have to be defined in clockwise direction";
        }

        return std::nullopt;
    }

    void Polygon::Init()
    {
        Primitive::Init();
    }

    void Polygon::OnPositionChange( const sf::Vector2f& newPosition )
    {
        auto verticesN = m_vertexArray.getVertexCount();
        for( auto i = 0U; i < verticesN; ++i )
        {
            m_vertexArray[ i ].position = m_vertexArray[ i ].position - m_appliedMove + m_position;
        }
        m_appliedMove = m_position;
        Primitive::OnPositionChange( newPosition );
    }

    void Polygon::OnSizeChange( const sf::Vector2f& newSize )
    {
    }

    void Polygon::OnPointsChange( const PointsVector& newPoints )
    {
    }

    void Polygon::Triangulate()
    {
        m_vertexArray.clear();
        if( m_points.empty() )
        {
            return;
        }
        auto triangles = TriangleVector();
        auto polygonVertices = PointsList( m_points.cbegin(), m_points.cend() );
        auto adv = [&polygonVertices]( PointsListIter& iter ) {
            std::advance( iter, 1 );
            if( iter == polygonVertices.cend() )
            {
                iter = polygonVertices.cbegin();
            }
        };

        auto prev = polygonVertices.cbegin();
        auto curr = std::next( prev );
        auto next = std::next( curr );
        for( ; polygonVertices.size() != 2; )
        {
            if( IsEar( polygonVertices, prev, curr, next ) )
            {
                triangles.emplace_back( *prev, *curr, *next );
                polygonVertices.erase( curr );
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

        auto n = triangles.size();
        auto i = 0U;
        for( const auto& triangle : triangles )
        {
            auto f = []( float p ) {
                p = fmod( p, 1.0f );
                if( p <= 1.0f / 6.0f )
                {
                    return p * 6.0f;
                }
                else if( p > 1.0f / 6.0f && p <= 3.0f / 6.0f )
                {
                    return 1.0f;
                }
                else if( p > 3.0f / 6.0f && p < 4.0f / 6.0f )
                {
                    return 4.0f + ( -p ) * 6.0f;
                }
                else
                {
                    return 0.0f;
                }
            };
            auto p = float( i ) / n;
            auto r = 255U * f( p - 4.0f / 6.0f + 1.0f );
            auto g = 255U * f( p );
            auto b = 255U * f( p - 2.0f / 6.0f + 1.0f );
            m_vertexArray.append( sf::Vertex( std::get< 0 >( triangle ), sf::Color( sf::Color( r, g, b ) ) ) );
            m_vertexArray.append( sf::Vertex( std::get< 1 >( triangle ), sf::Color( sf::Color::Transparent ) ) );
            m_vertexArray.append( sf::Vertex( std::get< 2 >( triangle ), sf::Color( sf::Color::Transparent ) ) );
            ++i;
        }
    }

    bool Polygon::IsEar( const PointsList& polygonVertices, PointsListIter previousVertex, PointsListIter currentVertex, PointsListIter nextVertex ) const
    {
        auto angle = GetAngle( *previousVertex, *currentVertex, *nextVertex );
        if( angle >= Resource::Pi )
        {
            return false;
        }

        auto it = polygonVertices.cbegin();
        for( ; it != polygonVertices.cend(); ++it )
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

    float Polygon::GetAngle( const Point& previousVertex, const Point& currentVertex, const Point& nextVertex )
    {
        auto prev = previousVertex - currentVertex;
        auto next = nextVertex - currentVertex;
        return fmodf( atan2f( prev.y, prev.x ) - atan2f( next.y, next.x ) + 2.0f * Resource::Pi, 2.0f * Resource::Pi );
    }

    bool Polygon::IsPointInsideTriangle( const Point& examinedPoint, const Point& a, const Point& b, const Point& c ) const
    {
        return GetAngle( examinedPoint, a, b ) < Resource::Pi && GetAngle( examinedPoint, b, c ) < Resource::Pi && GetAngle( examinedPoint, c, a ) < Resource::Pi;
    }

    bool Polygon::AreLinesOverlapped( const std::pair< const Point&, const Point& >& lineA, const std::pair< const Point&, const Point& >& lineB )
    {
        return ( ( GetAngle( lineA.first, lineA.second, lineB.first ) < Resource::Pi ) != ( GetAngle( lineA.first, lineA.second, lineB.second ) < Resource::Pi ) ) &&
               ( ( GetAngle( lineB.first, lineB.second, lineA.first ) < Resource::Pi ) != ( GetAngle( lineB.first, lineB.second, lineA.second ) < Resource::Pi ) );
    }
}
