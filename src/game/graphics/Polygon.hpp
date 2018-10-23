#pragma once
#include "game/graphics/Primitive.hpp"

#include <vector>

namespace Graphics
{
    class Polygon final : public Primitive
    {
    public:
        using PointsVector = std::vector< sf::Vector2f >;

        Polygon();
        ~Polygon();

        void Render( sf::RenderTarget& target );

        Utils::Property< PointsVector > Points;

    private:
        void Init() override;

        void OnPositionChange( const sf::Vector2f& newPosition );
        void OnSizeChange( const sf::Vector2f& newSize );
    };
}