#pragma once
#include "game/GenericGame.hpp"

#include "game/graphics/Polygon.hpp"
#include "game/graphics/Text.hpp"

namespace Game
{
    class StateMachine;
}

namespace Betomnita
{
    class Cursor;

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

        static BetomnitaGame* GetInstance();

    private:
        void OnStart() override;
        void OnUpdate( sf::Time dt ) override;
        void OnRender( sf::RenderTarget& target ) override;
        void OnVideoSettingsChanged() override;
        void OnClose() override;
        void OnKeyPressed( const sf::Event::KeyEvent& key ) override;
        void OnKeyReleased( const sf::Event::KeyEvent& key ) override;
        void OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button ) override;
        void OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button ) override;
        void OnMouseMoved( const sf::Vector2f& position ) override;

        static BetomnitaGame* s_instance;
        std::unique_ptr< Game::StateMachine > m_flowController;
        sf::CircleShape m_circle;
        sf::RectangleShape m_rect;
        Graphics::Text m_text;
        Graphics::Polygon m_polygon;
        Graphics::Polygon::PointsVector m_points;
    };
}