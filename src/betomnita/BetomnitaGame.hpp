#pragma once
#include "game/GenericGame.hpp"

namespace Game
{
    template< class StateId >
    class StateMachine;
}

namespace Game::Graphics
{
    class Text;
}

namespace Betomnita
{
    class Cursor;

    class BetomnitaGame final : public Game::GenericGame
    {
    public:
        BetomnitaGame();
        ~BetomnitaGame();

        static BetomnitaGame* GetInstance();

    private:
        void OnStart() override;
        void OnUpdate( const sf::Time& dt ) override;
        void OnRender( sf::RenderTarget& target ) override;
        void OnVideoSettingsChanged() override;
        void OnClose() override;
        void OnKeyPressed( const sf::Event::KeyEvent& key ) override;
        void OnKeyReleased( const sf::Event::KeyEvent& key ) override;
        void OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button ) override;
        void OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button ) override;
        void OnMouseMoved( const sf::Vector2f& position ) override;

        static BetomnitaGame* s_instance;
        std::unique_ptr< Game::StateMachine< Resource::StateId > > m_stateMachine;
        std::unique_ptr< Cursor > m_cursor;
    };
}