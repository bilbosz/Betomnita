#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>

namespace Betomnita::GamePlay
{
    class View;
    class Cursor;
    class Terrain;
    class PhysicalBody;
    class TerrainPlatform;

    class World final
    {
    public:
        World();
        ~World();

        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );

        // Map (from file)
        // Minimap (getter, thumbnail)
        // Size (AABB of possible positions)
        // PhysialBodies (obstacles, vechicles and buildings)
        // Isohypse (terrain platforms)

    private:
        sf::FloatRect m_size;
        //std::unique_ptr< Terrain > m_terrainSheets;
        std::vector< std::unique_ptr< PhysicalBody > > m_physicalBodies;
        // std::vector< std::unique_ptr< TerrainPlatform > > m_isohypse;
    };
}