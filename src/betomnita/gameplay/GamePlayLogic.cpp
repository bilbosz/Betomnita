#include "betomnita/gameplay/GamePlayLogic.hpp"

#include "app/Debug.hpp"
#include "betomnita/resources/Resources.hpp"
#include "game/graphics/Polygon.hpp"
#include "betomnita/gameplay/World.hpp"

namespace Betomnita::GamePlay
{
    GamePlayLogic::GamePlayLogic() : m_world( std::make_unique< World >() )
    {
    }

    GamePlayLogic::~GamePlayLogic()
    {
    }

    void GamePlayLogic::Render( sf::RenderTarget& target )
    {
        m_world->Render( target );
    }

    void GamePlayLogic::Update( const sf::Time& dt )
    {
        m_world->Update( dt );
        m_time += dt;
    }
}