#pragma once
#include "betomnita/gameplay/Vehicle.hpp"
#include "game/graphics/View.hpp"
#include "game/utils/AABB.hpp"

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>

namespace Betomnita::GamePlay
{
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

        void Pause();
        void Unpause();

        void LoadFromFile( const std::string& filename );

        void InitView();

        void InitPhysics();
        b2World& PhysicsWorld()
        {
            return m_physicsWorld;
        }

    private:
        std::vector< std::unique_ptr< Terrain > > m_terrainSheets;
        GamePlayLogic* m_currentLogic;
        std::vector< Vehicle > m_vehicles;

        b2World m_physicsWorld;
        Game::Graphics::View m_view;
        sf::Vector2f m_previousPoint;
        bool m_moving = false;
        Game::Utils::AABB m_size;
    };
}