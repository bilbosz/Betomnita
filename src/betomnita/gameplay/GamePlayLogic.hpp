#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace Game::Graphics
{
    class Polygon;
}

namespace Betomnita::GamePlay
{
    class World;
    class Prototype;
    class Unit;
    class VechicleChassis;
    class PrototypeDict;

    class GamePlayLogic final
    {
    public:
        GamePlayLogic();
        ~GamePlayLogic();

        void Render( sf::RenderTarget& target );
        void Update( const sf::Time& dt );

        const PrototypeDict& GetPrototypeDict() const;

    private:
        sf::Time m_time;
        std::unique_ptr< World > m_world; // mainly physics and world design
        std::unique_ptr< PrototypeDict > m_prototypeDict; // map from images into cached prototype
        // std::vector< std::unique_ptr< VechicleChassis > > m_prototypes; // cached vechicle prototypes
        // std::vector< std::unique_ptr< Unit > > m_units; // for executing units commands
    };
}