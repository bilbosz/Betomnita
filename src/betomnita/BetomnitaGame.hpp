#pragma once
#include "game/GenericGame.hpp"
#include "game/StateMachine.hpp"

namespace Betomnita
{
    class BetomnitaGame final : public Game::GenericGame
    {
    public:
        enum class States
        {
            Undefined = -1,
            MainMenu,
            Gameplay,
            Pause,
        };

        BetomnitaGame();
        ~BetomnitaGame();
    
    private:
        void OnStart() override;
        void OnUpdate( sf::Time dt ) override;
        void OnRender( sf::Time dt ) override;
        void OnVideoSettingsChanged() override;
        void OnClose() override;
        void OnKeyPressed( const sf::Event::KeyEvent& key ) override;
        void OnKeyReleased( const sf::Event::KeyEvent& key ) override;
        void OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button ) override;
        void OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button ) override;
        void OnMouseMoved( const sf::Vector2f& position ) override;

        Game::StateMachine m_flowController;
        sf::CircleShape m_circle;
        sf::Text m_text;
        sf::RectangleShape m_rect;
    };
}