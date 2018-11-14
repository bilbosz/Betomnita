#pragma once
#include "game/graphics/Primitive.hpp"

#include <list>
#include <vector>

namespace Graphics
{
    /* Filled closed path which is described by points ordered clockwise. It is implied in this description that there is no holes. Before rendering polygon has to be
       divided into triangles. This class uses simple ear clipping algorithm for triangulation (based on two ears theorem) which runs in O( n ^ 3 ), where n is number of
       points. If there are application efficiency issues caused by this code it is highly recommended to rewrite this code into O( n * log( n ) ). */
    class Polygon final : public Primitive
    {
    public:
        using Point = sf::Vector2f;
        using PointsVector = std::vector< Point >;
        using Triangle = std::tuple< Point, Point, Point >;
        using TriangleVector = std::vector< Triangle >;
        using PointsList = std::list< Point >;
        using PointsListIter = PointsList::const_iterator;

        Polygon();
        ~Polygon();

        void Render( sf::RenderTarget& target ) override;

        const PointsVector& GetPoints() const
        {
            return m_points;
        }
        void SetPoints( const PointsVector& value );

    private:
        void Init() override;

        void OnPositionChange( const sf::Vector2f& newPosition );
        void OnSizeChange( const sf::Vector2f& newSize );
        void OnPointsChange( const PointsVector& newPoints );

        void Triangulate();
        bool IsEar( const PointsList& polygonVerticies, PointsListIter previousVertex, PointsListIter currentVertex, PointsListIter nextVertex ) const;
        float GetAngle( const Point& previousVertex, const Point& currentVertex, const Point& nextVertex ) const;
        bool IsPointInsideTriangle( const Point& examinedPoint, const Point& a, const Point& b, const Point& c ) const;

        PointsVector m_points;
        TriangleVector m_triangles;

        sf::VertexArray m_vertexArray;
    };
}