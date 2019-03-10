#include "AABB.hpp"

namespace Game::Utils
{
    bool AABB::Contains( const sf::Vector2f& point ) const
    {
        return point.x >= MinX && point.y >= MinY && point.x <= MaxX && point.y <= MaxY;
    }

    void AABB::Init( const sf::Vector2f& point )
    {
        MinX = point.x;
        MinY = point.y;
        MaxX = point.x;
        MaxY = point.y;
    }

    void AABB::Init( const AABB& aabb )
    {
        MinX = aabb.MinX;
        MinY = aabb.MinY;
        MaxX = aabb.MaxX;
        MaxY = aabb.MaxY;
    }

    void AABB::Constrain( const sf::Vector2f& point )
    {
        MinX = std::min( MinX, point.x );
        MinY = std::min( MinY, point.y );
        MaxX = std::max( MaxX, point.x );
        MaxY = std::max( MaxY, point.y );
    }

    void AABB::Constrain( const AABB& aabb )
    {
        MinX = std::min( MinX, aabb.MinX );
        MinY = std::min( MinY, aabb.MinY );
        MaxX = std::max( MaxX, aabb.MaxX );
        MaxY = std::max( MaxY, aabb.MaxY );
    }
}
