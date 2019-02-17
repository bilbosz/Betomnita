#include "SVGHelper.hpp"

#include "app/Debug.hpp"
#include "game/GameConsts.hpp"

namespace Game::Graphics
{
    std::vector< std::vector< sf::Vector2f > > SVGHelper::ParsePathDescriptions( const char* value )
    {
        std::vector< std::vector< sf::Vector2f > > result;

        auto it = value;
        auto cmd = '\0';
        auto isNumber = []( char ch ) { return isdigit( ch ) || ch == '-' || ch == '+' || ch == '.' || ch == 'e'; };
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
        std::vector< sf::Vector2f >* currentPolygon = nullptr;
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
                            if( toupper( cmd ) == 'M' )
                            {
                                currentPolygon = &result.emplace_back();
                            }
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
                    ASSERT( currentPolygon != nullptr, L"Path did not start with M command" );
                    currentPolygon->emplace_back( x, y );
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
        return result;
    }

    std::unordered_map< std::string, std::string > SVGHelper::ParseStyle( const char* value )
    {
        std::unordered_map< std::string, std::string > result;

        auto it = value;
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
                result.emplace( std::make_pair( std::string( keyBegin, keyEnd ), std::string( valueBegin, valueEnd ) ) );
                keyBegin = nullptr;
                keyEnd = nullptr;
                valueBegin = nullptr;
                valueEnd = nullptr;
            }
        }
        return result;
    }

    sf::Transform SVGHelper::ParseTransform( const char* value )
    {
        sf::Transform result;

        auto it = value;
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
                    result.combine( { args[ 0 ], args[ 2 ], args[ 4 ], args[ 1 ], args[ 3 ], args[ 5 ], 0.0f, 0.0f, 1.0f } );
                }
                else if( fun == "translate" )
                {
                    CHECK( args.size() == 1 || args.size() == 2 );
                    result.translate( args[ 0 ], args.size() == 2 ? args[ 1 ] : 0.0f );
                }
                else if( fun == "scale" )
                {
                    CHECK( args.size() == 1 || args.size() == 2 );
                    result.scale( args[ 0 ], args.size() == 2 ? args[ 1 ] : args[ 0 ] );
                }
                else if( fun == "rotate" )
                {
                    CHECK( args.size() == 1 || args.size() == 3 );
                    if( args.size() == 1 )
                    {
                        result.rotate( args[ 0 ] );
                    }
                    else
                    {
                        result.rotate( args[ 0 ], args[ 1 ], args[ 2 ] );
                    }
                }
                else if( fun == "skewx" )
                {
                    CHECK( args.size() == 1 );
                    result.combine( { 1.0f, tanf( args[ 0 ] * 180.0f / Game::Consts::Pi ), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f } );
                }
                else if( fun == "skewy" )
                {
                    CHECK( args.size() == 1 );
                    result.combine( { 1.0f, 0.0f, 0.0f, tanf( args[ 0 ] * 180.0f / Game::Consts::Pi ), 1.0f, 0.0f, 0.0f, 0.0f, 1.0f } );
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
        return result;
    }

    std::vector< std::string > SVGHelper::ParseClass( const char* value )
    {
        std::vector< std::string > result( ( std::istream_iterator< std::string >( std::istringstream( value ) ) ), std::istream_iterator< std::string >() );
        return result;
    }
}