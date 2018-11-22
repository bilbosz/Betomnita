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

        const sf::Transform& GetToScreenTransform() const
        {
            return m_modelToScreen;
        }
        const sf::Transform& GetToModelTransform() const
        {
            return m_screenToModel;
        }
        sf::Vector2f GetToScreenScale() const;
        sf::Vector2f GetToModelScale() const;
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
        virtual void OnScreenshotTaken( const std::string& path ) override;
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
        sf::Transform m_screenToModel;
        sf::Time m_gameTime;
    };
}