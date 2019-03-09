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
    class Unit;
    class GamePlayLogic;
    class VehicleChassis;
    class Vehicle;

    class World final
    {
    public:
        World( GamePlayLogic* logic );
        ~World();

        void Init();
        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );

        void LoadFromFile( const std::string& filename );

        sf::Transform GetView() const
        {
            return m_view;
        }

    private:
        sf::FloatRect m_size;
        std::vector< std::unique_ptr< Terrain > > m_terrainSheets;
        GamePlayLogic* m_currentLogic;
        std::vector< Vehicle > m_vehicles;
        
        sf::Transform m_view;
        sf::Vector2f m_previousPoint;
        bool m_moving = false;
        // std::vector< std::unique_ptr< PhysicalBody > > m_physicalBodies;
        // std::vector< std::unique_ptr< Unit > > m_units;
        // std::vector< std::unique_ptr< TerrainPlatform > > m_isohypse;
    };
}