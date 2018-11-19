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
            return L"Polygon has to have at least 3 verticies";
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
    }

    void Polygon::OnSizeChange( const sf::Vector2f& newSize )
    {
    }

    void Polygon::OnPointsChange( const PointsVector& newPoints )
    {
    }

    void Polygon::Triangulate()
    {
		m_triangles.clear();
        m_vertexArray.clear();
        if( m_points.empty() )
        {
            return;
        }
        auto polygonVerticies = PointsList( m_points.cbegin(), m_points.cend() );
        auto adv = [&polygonVerticies]( PointsListIter& iter ) {
            std::advance( iter, 1 );
            if( iter == polygonVerticies.cend() )
            {
                iter = polygonVerticies.cbegin();
            }
        };

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
            m_vertexArray.append( sf::Vertex( std::get< 0 >( triangle ), sf::Color( 0xFFC31EFF ) ) );
            m_vertexArray.append( sf::Vertex( std::get< 1 >( triangle ), sf::Color( 0xFFC31EFF ) ) );
            m_vertexArray.append( sf::Vertex( std::get< 2 >( triangle ), sf::Color( 0xFFC31EFF ) ) );
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
