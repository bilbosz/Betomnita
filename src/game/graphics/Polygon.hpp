#pragma once
#include "game/graphics/Primitive.hpp"

#include <vector>

namespace Graphics
{
	// Close polygon which is described by points ordered clockwise
    class Polygon final : public Primitive
    {
    public:
        using PointsVector = std::vector< sf::Vector2f >;

        Polygon();
        ~Polygon();

        void Render( sf::RenderTarget& target ) override;

        Utils::Property< PointsVector > Points;

    private:
        void Init() override;

        void OnPositionChange( const sf::Vector2f& newPosition );
        void OnSizeChange( const sf::Vector2f& newSize );
		void OnPointsChange( const PointsVector& newPoints );

		sf::VertexArray m_vertexArray;
    };
}