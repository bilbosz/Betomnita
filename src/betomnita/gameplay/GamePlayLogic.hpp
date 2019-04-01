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

        void Pause();
        void Unpause();

        const PrototypeDict& GetPrototypeDict() const;

    private:
        sf::Time m_time;
        std::unique_ptr< GamePlay::World > m_world;
        std::unique_ptr< PrototypeDict > m_prototypeDict;
    };
}