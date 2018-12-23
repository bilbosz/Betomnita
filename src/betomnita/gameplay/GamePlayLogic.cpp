#include "betomnita/gameplay/GamePlayLogic.hpp"

#include "app/Debug.hpp"

namespace Betomnita::GamePlay
{
    GamePlayLogic::GamePlayLogic()
    {
    }

    GamePlayLogic::~GamePlayLogic()
    {
    }

    void GamePlayLogic::Render( sf::RenderTarget& target )
    {
    }

    void GamePlayLogic::Update( const sf::Time& dt )
    {
        m_time += dt;
    }
}