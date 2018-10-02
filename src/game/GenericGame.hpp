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
        virtual void OnStart() override;
        virtual void OnUpdate( sf::Time dt ) override;
        virtual void OnRender( sf::Time dt ) override;
        virtual void OnVideoSettingsChanged() override;
        virtual void OnClose() override;
        virtual void OnKeyPressed( const sf::Event::KeyEvent& key );
        virtual void OnKeyReleased( const sf::Event::KeyEvent& key );
        virtual void OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button );
        virtual void OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button );
        virtual void OnMouseMoved( const sf::Vector2f& position );

        void OnEvent( const sf::Event& e ) override;

        const sf::Transform& GetTransformation() const { return m_modelToScreen; }
        float GetScale() const { return m_scale; }
        sf::Vector2f GetMousePosition() const;
		sf::Time GetGameTime() const;
    private:
        void UpdateModelToScreen();
        
        sf::Transform m_modelToScreen;
        float m_scale;
		sf::Time m_gameTime;
    };
}