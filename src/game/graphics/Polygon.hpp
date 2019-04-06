#pragma once
#include "game/graphics/Primitive.hpp"
#include "game/utils/AABB.hpp"

#include <array>
#include <list>
#include <optional>
#include <vector>

namespace pugi
{
    class xml_document;
    class xml_node;
}

namespace Game::Graphics
{
    /* Filled closed path which is described by points ordered counter clockwise. It is implied in this description that there is no holes. Before rendering polygon has
       to be divided into triangles. This class uses simple ear clipping algorithm for triangulation (based on two ears theorem) which runs in O( n ^ 3 ), where n is
       number of points. If there are application efficiency issues caused by this code it is highly recommended to rewrite this code into O( n * log( n ) ). */
    class Polygon final : public Primitive
    {
    public:
        enum class Error
        {
            NoError,
            NotEnoughVerticies,
            PointsNotUnique,
            LinesCrossing,
            WrongDirection,
            Size,
        };

        using Point = sf::Vector2f;
        using PointsVector = std::vector< Point >;
        using Triangle = std::array< Point, 3 >;
        using TriangleVector = std::vector< Triangle >;
        using PointsList = std::list< Point >;
        using PointsListIter = PointsList::const_iterator;

        Polygon();
        ~Polygon();

        static std::vector< Polygon > LoadManyFromSVGNode( const std::string& filename, pugi::xml_document& doc, pugi::xml_node& node, float scale );

        void Render( sf::RenderTarget& target ) override;

        const PointsVector& GetPoints() const
        {
            return m_points;
        }
        void SetPoints( const PointsVector& value );

        const sf::Color& GetColor() const
        {
            return m_color;
        }
        void SetColor( const sf::Color& value );

        void SetOutlineColor( const sf::Color& value );
        const sf::Color& GetOutlineColor() const
        {
            return m_outlineColor;
        }

        void SetOutlineThickness( float value );
        float GetOutlineThickness() const
        {
            return m_outlineThickness;
        }

        const Utils::AABB& GetAABB() const
        {
            return m_aabb;
        }

        static Error GetPointsErrors( const PointsVector& points );

        static bool IsRightDirection( const PointsVector& points );

        static std::vector< std::vector< sf::Vector2f > > Triangulate( const std::vector< sf::Vector2f >& points );

    private:
        static float GetAngle( const Point& previousVertex, const Point& currentVertex, const Point& nextVertex );
        static bool AreLinesOverlapped( const std::pair< const Point&, const Point& >& lineA, const std::pair< const Point&, const Point& >& lineB );

        static bool IsEar( const PointsList& polygonVertices, PointsListIter previousVertex, PointsListIter currentVertex, PointsListIter nextVertex );
        static bool IsPointInsideTriangle( const Point& examinedPoint, const Point& a, const Point& b, const Point& c );

        void Init() override;

        void OnPositionChange();
        void OnSizeChange();
        void OnPointsChange();

        void Triangulate();

        void ReversePoints();
        void CalculateAABB();

        std::vector< std::vector< sf::Vector2f > > GetTriangulatedPoints() const;

        PointsVector m_points;
        sf::Color m_color;

        sf::Color m_outlineColor;
        float m_outlineThickness;

        sf::Vector2f m_appliedMove;
        sf::VertexArray m_vertexArray;

        Utils::AABB m_aabb;
    };
}