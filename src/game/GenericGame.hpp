#pragma once
#include "app/Application.hpp"

namespace Game
{
    class GenericGame : public App::Application
    {
    public:
        GenericGame();
        ~GenericGame();

    protected:
        void OnStart() override;
        void OnUpdate( sf::Time dt ) override;
        void OnRender( sf::Time dt ) override;
        void OnEvent( const sf::Event& e ) override;
        void OnVideoSettingsChanged() override;
        void OnClose() override;
        void OnKeyPressed( const sf::Event::KeyEvent& key ) override;
        void OnKeyReleased( const sf::Event::KeyEvent& key ) override;
        void OnMouseButtonPressed( const sf::Event::MouseButtonEvent& mouseButton ) override;
        void OnMouseButtonReleased( const sf::Event::MouseButtonEvent& mouseButton ) override;
        void OnMouseMoved( const sf::Event::MouseMoveEvent& mouseMove ) override;

    private:
        void UpdateModelToScreen();

        sf::Transform m_modelToScreen;

        sf::CircleShape m_circle;
        sf::Text m_text;
        float m_modelScale;
    };
}