#pragma once
#include "game/GenericGame.hpp"

namespace Betomnita::Resources
{
    enum class StateId;
}

namespace Game
{
    template< class StateId >
    class StateMachine;
}

namespace Betomnita
{
    class Cursor;

    class BetomnitaGame final : public Game::GenericGame
    {
    public:
        BetomnitaGame();
        ~BetomnitaGame() final;

        static BetomnitaGame* GetInstance();
        Game::StateMachine< Resources::StateId >* GetStateMachine() const
        {
            return m_stateMachine.get();
        }

        std::unique_ptr< Betomnita::Cursor > Cursor;

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
        void OnMouseWheelScrolled( float delta ) override;

        static BetomnitaGame* s_instance;
        std::unique_ptr< Game::StateMachine< Resources::StateId > > m_stateMachine;
    };
}