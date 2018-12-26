#include "betomnita/gameplay/GamePlayLogic.hpp"

#include "app/Debug.hpp"
#include "betomnita/resources/Resources.hpp"
#include "game/graphics/Polygon.hpp"

namespace Betomnita::GamePlay
{
    GamePlayLogic::GamePlayLogic()
    {
        m_shapes = Game::Graphics::Polygon::LoadManyFromFile( Resources::World1 );
    }

    GamePlayLogic::~GamePlayLogic()
    {
    }

    void GamePlayLogic::Render( sf::RenderTarget& target )
    {
        for( auto& shape : m_shapes )
        {
            shape->Render( target );
        }
    }

    void GamePlayLogic::Update( const sf::Time& dt )
    {
        m_time += dt;
    }
}