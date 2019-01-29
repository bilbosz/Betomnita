#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Game::Graphics
{
    class Polygon;

    // Nodes:
    // svg
    // ellipse
    // path
    // image (svg reference)
    // g
    //
    // User attributes:
    // data-acceleration-point=
    // data-do-not-draw=
    // data-friction=
    // data-gun-rotator=
    // data-gun-rotator-slot=
    // data-physical-body-shape=
    // data-player=
    // data-scale=
    // data-shot-direction=
    // data-unit=
    //
    // Default attributes:
    // d=
    // style=
    // transform=

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
        static std::unordered_map< std::string, std::string > ParseStyle( const char* value );
        static sf::Transform ParseTransform( const char* value );
        static std::vector< std::string > ParseClass( const char* value );
    };
}