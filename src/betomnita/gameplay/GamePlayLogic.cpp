#include "betomnita/gameplay/GamePlayLogic.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/PrototypeDict.hpp"
#include "betomnita/gameplay/World.hpp"
#include "betomnita/resources/Resources.hpp"
#include "game/graphics/Polygon.hpp"

namespace Betomnita::GamePlay
{
    GamePlayLogic::GamePlayLogic() : m_world( std::make_unique< World >( this ) ), m_prototypeDict( std::make_unique< PrototypeDict >() )
    {
        m_prototypeDict->LoadFromDirectory( "res" );
        m_world->LoadFromFile( "res/worlds/1/def.svg" );
        m_world->Init();
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

    const PrototypeDict& GamePlayLogic::GetPrototypeDict() const
    {
        return *m_prototypeDict.get();
    }
}