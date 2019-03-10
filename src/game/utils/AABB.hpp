#pragma once
#include <SFML/Graphics.hpp>

namespace Game::Utils
{
    struct AABB
    {
        float MinX;
        float MinY;
        float MaxX;
        float MaxY;

        bool Contains( const sf::Vector2f& point ) const;

        void Init( const sf::Vector2f& point );
        void Init( const AABB& aabb );

        void Constrain( const sf::Vector2f& point );
        void Constrain( const AABB& aabb );
        template< class Range >

        void Evaluate( const Range& range );
    };

    template< class Range >
    void AABB::Evaluate( const Range& range )
    {
        Init( *range.begin() );
        for( const auto& item : range )
        {
            Constrain( item );
        }
    }
}
