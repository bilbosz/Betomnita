#pragma once
#include "game/graphics/Primitive.hpp"

#include <vector>

namespace Graphics
{
    /* Close polygon which is described by points ordered clockwise. It is implied in this description that there is no holes. 
	   Before rendering polygon has to be divided into triangles. This class uses simple ear clipping algorithm for 
	   triangulation which runs in O(n^3), where n is number of points. If there are application efficiency issues caused by
	   this code it is highly recommended to rewrite this code into O(n*log(n)). */
    class Polygon final : public Primitive
    {
    public:
        using Point = sf::Vector2f;
        using PointsVector = std::vector< Point >;

        Polygon();
        ~Polygon();

        void Render( sf::RenderTarget& target ) override;

        const PointsVector& GetPoints() const;

    private:
        void Init() override;

        void OnPositionChange( const sf::Vector2f& newPosition );
        void OnSizeChange( const sf::Vector2f& newSize );
        void OnPointsChange( const PointsVector& newPoints );

        PointsVector m_perimeter;

        sf::VertexArray m_vertexArray;
    };
}