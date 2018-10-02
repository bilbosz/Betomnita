#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/MainMenuState.hpp"

namespace Betomnita
{
    BetomnitaGame::BetomnitaGame()
    {
        m_flowController.RegisterState( 0, std::make_shared< MainMenuState >() );
        m_flowController.PushState( 0 );
    }

    BetomnitaGame::~BetomnitaGame() {}

    void BetomnitaGame::OnUpdate( sf::Time dt ) { m_flowController.OnUpdate( dt ); }
}