#include "game/graphics/Polygon.hpp"

#include "app/Debug.hpp"
#include "game/GameConsts.hpp"
#include "game/GenericGame.hpp"
#include "game/graphics/SVGHelper.hpp"

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

    std::vector< std::unique_ptr< Polygon > > Polygon::LoadManyFromSVGNode( const std::string& filename, const pugi::xml_node& node, float scale )
    {
        std::vector< std::unique_ptr< Polygon > > result;
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

        for( auto& points : desc )
        {
            auto& polygon = result.emplace_back( std::make_unique< Polygon >() );
            for( auto& point : points )
            {
                point = transform.transformPoint( point );
            }
#ifdef DEBUG
            bool samePoints = false;
#endif
            while( !points.empty() && *points.cbegin() == *points.crbegin() )
            {
                points.pop_back();
#ifdef DEBUG
                samePoints = true;
#endif
            }
#ifdef DEBUG
            if( samePoints )
            {
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
            if( !IsRightDirection( points ) )
            {
                WARNING( L"Verticies for polygon \"" << id << L"\" in file " << filename.c_str() << L" have to be reversed." );
                std::reverse( points.begin(), points.end() );
            }
            polygon->SetPoints( points );

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
            polygon->SetColor( sf::Color( fillColor ) );
        }
        return result;
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
            CHECK( GetPointsErrors( value ) == Error::NoError );
        }
#endif
        m_points = value;
        OnPointsChange();
        Triangulate();
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
        return fmodf( atan2f( next.y, next.x ) - atan2f( prev.y, prev.x ) + 2.0f * Game::Consts::Pi, 2.0f * Game::Consts::Pi );
    }

    bool Polygon::AreLinesOverlapped( const std::pair< const Point&, const Point& >& lineA, const std::pair< const Point&, const Point& >& lineB )
    {
        return ( ( GetAngle( lineA.first, lineA.second, lineB.first ) < Game::Consts::Pi ) !=
                 ( GetAngle( lineA.first, lineA.second, lineB.second ) < Game::Consts::Pi ) ) &&
               ( ( GetAngle( lineB.first, lineB.second, lineA.first ) < Game::Consts::Pi ) !=
                 ( GetAngle( lineB.first, lineB.second, lineA.second ) < Game::Consts::Pi ) );
    }

    bool Polygon::IsPointInsideTriangle( const Point& examinedPoint, const Point& a, const Point& b, const Point& c ) const
    {
        return GetAngle( examinedPoint, a, b ) < Game::Consts::Pi && GetAngle( examinedPoint, b, c ) < Game::Consts::Pi &&
               GetAngle( examinedPoint, c, a ) < Game::Consts::Pi;
    }

    void Polygon::ParseDescription( const char* description )
    {
        auto it = description;
        auto cmd = '\0';
        auto isNumber = []( char ch ) { return isdigit( ch ) || ch == '-' || ch == '+' || ch == '.'; };
        auto alterCoord = [&cmd, &it]( float& coord ) {
            auto val = static_cast< float >( atof( it ) );
            if( isupper( cmd ) )
            {
                coord = val;
            }
            else
            {
                coord += val;
            }
        };
        auto x = 0.0f;
        auto y = 0.0f;
        auto isXLoaded = false;
        while( *it )
        {
            if( isalpha( *it ) )
            {
                cmd = *it;
                ++it;
            }
            else if( isNumber( *it ) )
            {
                switch( toupper( cmd ) )
                {
                    case 'M':
                    case 'L':
                        if( isXLoaded )
                        {
                            isXLoaded = false;
                            alterCoord( y );
                        }
                        else
                        {
                            isXLoaded = true;
                            alterCoord( x );
                        }
                        break;
                    case 'H':
                        alterCoord( x );
                        break;
                    case 'V':
                        alterCoord( y );
                        break;
                    case 'Z':
                        break;
                    default:
                        ASSERT( false, L"'" << toupper( cmd ) << "' is wrong command for polygon description" );
                }
                if( !isXLoaded )
                {
                    m_points.emplace_back( x, y );
                }
                do
                {
                    ++it;
                } while( isNumber( *it ) );
            }
            else
            {
                ++it;
            }
        }
    }

    void Polygon::ParseTransformation( const char* transformation, float scale )
    {
        sf::Transform transform;
        transform.scale( scale, scale );

        auto it = transformation;
        const char* funBegin = nullptr;
        const char* funEnd = nullptr;
        std::vector< float > args;
        auto isNumber = []( char ch ) { return isdigit( ch ) || ch == '-' || ch == '+' || ch == '.'; };

        for( ; *it; ++it )
        {
            if( isspace( *it ) || *it == '(' || *it == ',' )
            {
                continue;
            }
            if( !funBegin )
            {
                funBegin = it;
            }
            else if( !funEnd )
            {
                while( *it && !isspace( *it ) && *it != '(' )
                {
                    ++it;
                }
                funEnd = it;
            }
            else if( isNumber( *it ) )
            {
                args.emplace_back( static_cast< float >( atof( it ) ) );
                do
                {
                    ++it;
                } while( isNumber( *it ) );
                --it;
            }
            else if( *it == ')' )
            {
                std::string fun( funBegin, funEnd );
                std::transform( fun.begin(), fun.end(), fun.begin(), tolower );
                if( fun == "matrix" )
                {
                    CHECK( args.size() == 6 );
                    transform.combine( { args[ 0 ], args[ 2 ], args[ 4 ], args[ 1 ], args[ 3 ], args[ 5 ], 0.0f, 0.0f, 1.0f } );
                }
                else if( fun == "translate" )
                {
                    CHECK( args.size() == 1 || args.size() == 2 );
                    transform.translate( args[ 0 ], args.size() == 2 ? args[ 1 ] : 0.0f );
                }
                else if( fun == "scale" )
                {
                    CHECK( args.size() == 1 || args.size() == 2 );
                    transform.scale( args[ 0 ], args.size() == 2 ? args[ 1 ] : args[ 0 ] );
                }
                else if( fun == "rotate" )
                {
                    CHECK( args.size() == 1 || args.size() == 3 );
                    if( args.size() == 1 )
                    {
                        transform.rotate( args[ 0 ] );
                    }
                    else
                    {
                        transform.rotate( args[ 0 ], args[ 1 ], args[ 2 ] );
                    }
                }
                else if( fun == "skewx" )
                {
                    CHECK( args.size() == 1 );
                    transform.combine( { 1.0f, tanf( args[ 0 ] * 180.0f / Game::Consts::Pi ), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f } );
                }
                else if( fun == "skewy" )
                {
                    CHECK( args.size() == 1 );
                    transform.combine( { 1.0f, 0.0f, 0.0f, tanf( args[ 0 ] * 180.0f / Game::Consts::Pi ), 1.0f, 0.0f, 0.0f, 0.0f, 1.0f } );
                }
                else
                {
                    ASSERT( false, L"There is no \"" << fun.c_str() << "\" SVG transformation" );
                }
                funBegin = nullptr;
                funEnd = nullptr;
                args.clear();
            }
        }
        for( auto& point : m_points )
        {
            point = transform.transformPoint( point );
        }
    }

    void Polygon::ParseStyle( const char* style )
    {
        auto it = style;
        std::unordered_map< std::string, std::string > styles;
        const char* keyBegin = nullptr;
        const char* keyEnd = nullptr;
        const char* valueBegin = nullptr;
        const char* valueEnd = nullptr;
        for( ; *it; ++it )
        {
            if( isspace( *it ) || *it == ':' )
            {
                continue;
            }
            if( !keyBegin )
            {
                if( isalpha( *it ) || *it == '-' )
                {
                    keyBegin = it;
                }
            }
            else if( !keyEnd )
            {
                while( *it && !isspace( *it ) && *it != ':' )
                {
                    ++it;
                }
                CHECK( *it );
                keyEnd = it;
            }
            else if( !valueBegin )
            {
                valueBegin = it;
            }
            else if( !valueEnd )
            {
                while( *it && !isspace( *it ) && *it != ';' )
                {
                    ++it;
                }
                valueEnd = it;
                --it;
                styles.emplace( std::make_pair( std::string( keyBegin, keyEnd ), std::string( valueBegin, valueEnd ) ) );
                keyBegin = nullptr;
                keyEnd = nullptr;
                valueBegin = nullptr;
                valueEnd = nullptr;
            }
        }

        const auto& color = styles.find( "fill" );
        uint32_t colorVal = 0xffffffff;
        if( color != styles.cend() )
        {
            std::istringstream( color->second.substr( 1 ) ) >> std::hex >> colorVal;
            colorVal = colorVal << 8 | 0xff;
        }

        const auto& opacity = styles.find( "opacity" );
        float opacityVal;
        if( opacity != styles.cend() )
        {
            opacityVal = static_cast< float >( atof( opacity->second.c_str() ) );
            colorVal &= UINT_MAX << 8 | static_cast< uint32_t >( opacityVal * 255.0f );
        }
        SetColor( sf::Color( colorVal ) );
    }

    void Polygon::ReversePoints()
    {
        std::reverse( m_points.begin(), m_points.end() );
    }
}
