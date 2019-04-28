#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Game::Graphics
{
    class Polygon;

    class SVGHelper final
    {
    public:
        SVGHelper() = delete;
        SVGHelper( const SVGHelper& ) = delete;
        SVGHelper( SVGHelper&& ) = delete;
        SVGHelper& operator=( const SVGHelper& ) = delete;
        SVGHelper& operator=( SVGHelper&& ) = delete;
        ~SVGHelper() = delete;

        static std::vector< std::vector< sf::Vector2f > > ParsePathDescriptions( const char* value );
        static std::string ConstructPathDescriptionString( const std::vector< std::vector< sf::Vector2f > >& value );
        static std::unordered_map< std::string, std::string > ParseStyle( const char* value );
        static sf::Transform ParseTransform( const char* value );
        static std::vector< std::string > ParseClass( const char* value );
    };
}