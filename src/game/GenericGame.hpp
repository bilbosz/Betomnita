#pragma once
#include "app/Application.hpp"

namespace Game
{
    class GenericGame : public App::Application
    {
    public:
        GenericGame();
        ~GenericGame();

        static GenericGame* GetInstance();

        const sf::Transform& GetTransformation() const
        {
            return m_modelToScreen;
        }
        sf::Vector2f GetScale() const;
        sf::Vector2f GetMousePosition() const;
        sf::Time GetGameTime() const
        {
            return m_gameTime;
        };

    protected:
        virtual void OnStart() override;
        virtual void OnUpdate( sf::Time dt ) override;
        virtual void OnRender( sf::RenderTarget& target ) override;
        virtual void OnVideoSettingsChanged() override;
        virtual void OnClose() override;
        virtual void OnKeyPressed( const sf::Event::KeyEvent& key );
        virtual void OnKeyReleased( const sf::Event::KeyEvent& key );
        virtual void OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button );
        virtual void OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button );
        virtual void OnMouseMoved( const sf::Vector2f& position );

        void OnEvent( const sf::Event& e ) override;

    private:
        void UpdateModelToScreen();

        static GenericGame* s_instance;
        sf::Transform m_modelToScreen;
        sf::Time m_gameTime;
    };
}