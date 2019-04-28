#include "game/graphics/Polygon.hpp"

#include "app/Debug.hpp"
#include "game/GameConsts.hpp"
#include "game/GenericGame.hpp"
#include "game/graphics/SVGHelper.hpp"
#include "project/Config.hpp"

#include <pugixml.hpp>
#include <sstream>
#include <unordered_map>

namespace Game::Graphics
{
    Polygon::Polygon() : m_vertexArray( sf::PrimitiveType::Triangles )
    {
    }

    Polygon::~Polygon()
    {
    }

    std::vector< Polygon > Polygon::LoadManyFromSVGNode( const std::string& filename, pugi::xml_document& doc, pugi::xml_node& node, float scale )
    {
        std::vector< Polygon > result;
#ifdef DEBUG
        auto id = node.attribute( "id" ).as_string();
#endif
        sf::Transform transform;
        auto desc = SVGHelper::ParsePathDescriptions( node.attribute( "d" ).as_string() );
        auto style = SVGHelper::ParseStyle( node.attribute( "style" ).as_string() );
        transform.scale( { scale, scale } );
        transform.combine( SVGHelper::ParseTransform( node.attribute( "transform" ).as_string() ) );

        auto current = &node.parent();
        while( *current->name() == '\0' )
        {
            transform.combine( SVGHelper::ParseTransform( current->attribute( "transform" ).as_string() ) );
            current = &current->parent();
        }
#ifdef FIX_POLYGON
        bool fixNeeded = false;
#endif
        for( auto& points : desc )
        {
#ifdef FIX_POLYGON
            fixNeeded = fixNeeded || points.size() != 3;
            bool samePoints = false;

            while( !points.empty() && *points.cbegin() == *points.crbegin() )
            {
                points.pop_back();
                samePoints = true;
            }
#endif
            VERIFY( points.size() >= 3 );
#ifdef FIX_POLYGON
            if( samePoints )
            {
                fixNeeded = true;
                WARNING( L"Verticies on the end of the path \"" << id << L"\" in file " << filename.c_str() << L" are the same as on the beginning." );
            }
#endif

#ifdef DEBUG
            if( App::Debug::IsExpensive() )
            {
                auto error = GetPointsErrors( points );
                switch( error )
                {
                    case Error::PointsNotUnique:
                        ASSERT( false, L"Polygon \"" << id << L"\" in file " << filename.c_str() << L" have not unique points." );
                        break;
                    case Error::LinesCrossing:
                        ASSERT( false, L"Polygon \"" << id << L"\" in file " << filename.c_str() << L" have lines crossing." );
                        break;
                }
            }
#endif

            auto& polygon = result.emplace_back();
            for( auto& point : points )
            {
                point = transform.transformPoint( point );
            }

#ifdef FIX_POLYGON
            if( !IsRightDirection( points ) )
            {
                fixNeeded = true;
                WARNING( L"Verticies for polygon \"" << id << L"\" in file " << filename.c_str() << L" have to be reversed." );
                std::reverse( points.begin(), points.end() );
            }
#endif

            polygon.SetPoints( points );

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
            polygon.SetColor( sf::Color( fillColor ) );
        }
#ifdef FIX_POLYGON
        if( fixNeeded )
        {
            sf::Transform inversedTransform = transform.getInverse();
#    ifdef SAVE_POLYGON_TRIANGULATION
            std::vector< std::vector< sf::Vector2f > > triangulatedPoints;
            for( auto& polygon : result )
            {
                auto triangles = polygon.GetTriangulatedPoints();
                for( auto& triangle : triangles )
                {
                    if( !IsRightDirection( triangle ) )
                    {
                        std::reverse( triangle.begin(), triangle.end() );
                    }
                    for( auto& point : triangle )
                    {
                        point = inversedTransform.transformPoint( point );
                    }
                }
                std::move( triangles.begin(), triangles.end(), std::back_inserter( triangulatedPoints ) );
            }
            node.attribute( "d" ).set_value( SVGHelper::ConstructPathDescriptionString( triangulatedPoints ).c_str() );
#    else
            for( auto& points : desc )
            {
                for( auto& point : points )
                {
                    point = inversedTransform.transformPoint( point );
                }
            }
            node.attribute( "d" ).set_value( SVGHelper::ConstructPathDescriptionString( desc ).c_str() );
#    endif
            doc.save_file( filename.c_str() );
        }
#endif

        return result;
    }

    void Polygon::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        if( m_vertexArray.getVertexCount() >= 3 )
        {
            sf::Transform temporary = sf::Transform::Identity;
            temporary.combine( Game::GenericGame::GetInstance()->GetToScreenTransform() );
            temporary.combine( transform );
            target.draw( m_vertexArray, temporary );
        }
    }

    void Polygon::SetPoints( const PointsVector& value )
    {
#ifdef DEBUG
        if( App::Debug::IsExpensive() )
        {
            CHECK( GetPointsErrors( value ) == Error::NoError );
        }
#endif
        m_points = value;
        OnPointsChange();
        Triangulate();
        CalculateAABB();
    }

    void Polygon::SetColor( const sf::Color& value )
    {
        m_color = value;
        auto verticesN = m_vertexArray.getVertexCount();
        for( size_t i = 0; i < verticesN; ++i )
        {
            m_vertexArray[ i ].color = m_color;
        }
    }

    void Polygon::SetOutlineColor( const sf::Color& value )
    {
        m_outlineColor = value;
    }

    void Polygon::SetOutlineThickness( float value )
    {
        m_outlineThickness = value;
    }

    Polygon::Error Polygon::GetPointsErrors( const PointsVector& points )
    {
        auto pointsN = points.size();
        if( pointsN == 0 )
        {
            return Error::NoError;
        }
        if( pointsN < 3 )
        {
            return Error::NotEnoughVerticies;
        }

        for( auto i = 0; i < pointsN; ++i )
        {
            for( auto j = i + 1; j < pointsN; ++j )
            {
                if( points[ i ] == points[ j ] )
                {
                    return Error::PointsNotUnique;
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
                    return Error::LinesCrossing;
                }
            }
        }

        if( !IsRightDirection( points ) )
        {
            return Error::WrongDirection;
        }

        return Error::NoError;
    }

    bool Polygon::IsRightDirection( const PointsVector& points )
    {
        auto pointsN = points.size();
        auto angleSum = 0.0f;
        for( auto i = 0; i < pointsN; ++i )
        {
            angleSum += GetAngle( points[ i ], points[ ( i + 1 ) % pointsN ], points[ ( i + 2 ) % pointsN ] );
        }
        return angleSum < pointsN * Game::Consts::Pi;
    }

    std::vector< std::vector< sf::Vector2f > > Polygon::Triangulate( const std::vector< sf::Vector2f >& points )
    {
        std::vector< std::vector< sf::Vector2f > > result;
        auto error = GetPointsErrors( points );
        ASSERT( error == Error::NoError, L"There is error in points" );
        if( points.empty() )
        {
            return result;
        }
        auto polygonVertices = PointsList( points.cbegin(), points.cend() );
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
                result.emplace_back( std::vector< sf::Vector2f >{ *prev, *curr, *next } );
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
        return result;
    }

    void Polygon::Init()
    {
        Primitive::Init();
    }

    void Polygon::OnPositionChange()
    {
        auto verticesN = m_vertexArray.getVertexCount();
        for( auto i = 0u; i < verticesN; ++i )
        {
            m_vertexArray[ i ].position = m_vertexArray[ i ].position - m_appliedMove + m_position - m_pivot;
        }
        m_appliedMove = m_position - m_pivot;
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
                triangles.emplace_back( Triangle( { *prev, *curr, *next } ) );
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

    float Polygon::GetAngle( const Point& previousVertex, const Point& currentVertex, const Point& nextVertex )
    {
        auto prev = previousVertex - currentVertex;
        auto next = nextVertex - currentVertex;
        return fmodf( atan2f( next.y, next.x ) - atan2f( prev.y, prev.x ) + 2.0f * Game::Consts::Pi, 2.0f * Game::Consts::Pi );
    }

    bool Polygon::AreLinesOverlapped( const std::pair< const Point&, const Point& >& lineA, const std::pair< const Point&, const Point& >& lineB )
    {
        return ( ( GetAngle( lineA.first, lineA.second, lineB.first ) < Game::Consts::Pi ) !=
                 ( GetAngle( lineA.first, lineA.second, lineB.second ) < Game::Consts::Pi ) ) &&
               ( ( GetAngle( lineB.first, lineB.second, lineA.first ) < Game::Consts::Pi ) !=
                 ( GetAngle( lineB.first, lineB.second, lineA.second ) < Game::Consts::Pi ) );
    }

    bool Polygon::IsEar( const PointsList& polygonVertices, PointsListIter previousVertex, PointsListIter currentVertex, PointsListIter nextVertex )
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

    bool Polygon::IsPointInsideTriangle( const Point& examinedPoint, const Point& a, const Point& b, const Point& c )
    {
        return GetAngle( examinedPoint, a, b ) < Game::Consts::Pi && GetAngle( examinedPoint, b, c ) < Game::Consts::Pi &&
               GetAngle( examinedPoint, c, a ) < Game::Consts::Pi;
    }

    void Polygon::ReversePoints()
    {
        std::reverse( m_points.begin(), m_points.end() );
    }

    void Polygon::CalculateAABB()
    {
        m_aabb.Evaluate( m_points );
    }

    std::vector< std::vector< sf::Vector2f > > Polygon::GetTriangulatedPoints() const
    {
        std::vector< std::vector< sf::Vector2f > > result;
        auto verticesN = m_vertexArray.getVertexCount();
        for( auto i = 0u; i < verticesN; i += 3 )
        {
            result.emplace_back( std::vector< sf::Vector2f >{ m_vertexArray[ i ].position, m_vertexArray[ i + 1 ].position, m_vertexArray[ i + 2 ].position } );
        }
        return result;
    }
}
