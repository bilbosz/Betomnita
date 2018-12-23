#include "game/graphics/Polygon.hpp"

#include "app/Debug.hpp"
#include "game/GameConsts.hpp"
#include "game/GenericGame.hpp"

#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>
#undef NANOSVG_IMPLEMENTATION

namespace Game::Graphics
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
        OnPointsChange();
        Triangulate();
    }

    void Polygon::LoadFromFile( const std::string& path, float scale /* = 1.0f */ )
    {
        m_points.clear();

        NSVGimage* svg = nsvgParseFromFile( path.c_str(), "px", 96.0f );
        VERIFY( svg );
        ASSERT( svg->shapes->next == NULL, L"There should be only one shape in file" );
        NSVGshape* shape = svg->shapes;
        VERIFY( shape );
        ASSERT( shape->paths->next == NULL, L"There should be only one path describing polygon" );
        NSVGpath* svgPath = shape->paths;
        VERIFY( svgPath );

        for( auto i = 0; i < svgPath->npts - 1; i += 3 )
        {
            m_points.push_back( { svgPath->pts[ 2 * i ] * scale, svgPath->pts[ 2 * i + 1 ] * scale } );
        }
        if( shape->fill.type == NSVG_PAINT_COLOR )
        {
            sf::Color color;
            color.r = ( shape->fill.color >> 0 ) & 255;
            color.g = ( shape->fill.color >> 8 ) & 255;
            color.b = ( shape->fill.color >> 16 ) & 255;
            color.a = static_cast< sf::Uint8 >( shape->opacity * 255 );
            SetColor( color );
        }
        nsvgDelete( svg );
#ifdef DEBUG
        if( App::Debug::IsExpensive() )
        {
            auto error = GetPointsErrors( m_points );
            ASSERT( !error.has_value(), error.value() );
        }
#endif
        OnPointsChange();
        Triangulate();
    }

    void Polygon::SetColor( const sf::Color& value )
    {
        m_color = value;
        auto verticesN = m_vertexArray.getVertexCount();
        for( auto i = 0U; i < verticesN; ++i )
        {
            m_vertexArray[ i ].color = m_color;
        }
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
        if( angleSum >= pointsN * Game::Consts::Pi )
        {
            return L"Points have to be defined in clockwise direction";
        }

        return std::nullopt;
    }

    void Polygon::Init()
    {
        Primitive::Init();
    }

    void Polygon::OnPositionChange()
    {
        auto verticesN = m_vertexArray.getVertexCount();
        for( auto i = 0U; i < verticesN; ++i )
        {
            m_vertexArray[ i ].position = m_vertexArray[ i ].position - m_appliedMove + m_position;
        }
        m_appliedMove = m_position;
        Primitive::OnPositionChange();
    }

    void Polygon::OnSizeChange()
    {
    }

    void Polygon::OnPointsChange()
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

        for( const auto& triangle : triangles )
        {
            m_vertexArray.append( sf::Vertex( std::get< 0 >( triangle ), m_color ) );
            m_vertexArray.append( sf::Vertex( std::get< 1 >( triangle ), m_color ) );
            m_vertexArray.append( sf::Vertex( std::get< 2 >( triangle ), m_color ) );
        }
    }

    bool Polygon::IsEar( const PointsList& polygonVertices, PointsListIter previousVertex, PointsListIter currentVertex, PointsListIter nextVertex ) const
    {
        auto angle = GetAngle( *previousVertex, *currentVertex, *nextVertex );
        if( angle >= Game::Consts::Pi )
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
        return fmodf( atan2f( prev.y, prev.x ) - atan2f( next.y, next.x ) + 2.0f * Game::Consts::Pi, 2.0f * Game::Consts::Pi );
    }

    bool Polygon::IsPointInsideTriangle( const Point& examinedPoint, const Point& a, const Point& b, const Point& c ) const
    {
        return GetAngle( examinedPoint, a, b ) < Game::Consts::Pi && GetAngle( examinedPoint, b, c ) < Game::Consts::Pi &&
               GetAngle( examinedPoint, c, a ) < Game::Consts::Pi;
    }

    bool Polygon::AreLinesOverlapped( const std::pair< const Point&, const Point& >& lineA, const std::pair< const Point&, const Point& >& lineB )
    {
        return ( ( GetAngle( lineA.first, lineA.second, lineB.first ) < Game::Consts::Pi ) !=
                 ( GetAngle( lineA.first, lineA.second, lineB.second ) < Game::Consts::Pi ) ) &&
               ( ( GetAngle( lineB.first, lineB.second, lineA.first ) < Game::Consts::Pi ) !=
                 ( GetAngle( lineB.first, lineB.second, lineA.second ) < Game::Consts::Pi ) );
    }
}
