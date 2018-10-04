#include "game/GenericGame.hpp"

#include "app/Debug.hpp"

#include <SFML/Graphics.hpp>

namespace Game
{
    GenericGame::GenericGame() {}

    GenericGame::~GenericGame() {}

    void GenericGame::OnStart() {}

    void GenericGame::OnUpdate( sf::Time dt ) { m_gameTime += dt; }

    void GenericGame::OnRender( sf::RenderTarget& target ) {}

    void GenericGame::OnEvent( const sf::Event& e )
    {
        switch( e.type )
        {
            case sf::Event::EventType::Closed:
            {
                RequestShutdown();
            }
            break;
            case sf::Event::EventType::KeyPressed:
            {
                OnKeyPressed( e.key );
            }
            break;
            case sf::Event::EventType::KeyReleased:
            {
                OnKeyReleased( e.key );
            }
            case sf::Event::EventType::MouseButtonPressed:
            {
                OnMouseButtonPressed(
                    m_modelToScreen.getInverse().transformPoint( static_cast< float >( e.mouseButton.x ), static_cast< float >( e.mouseButton.y ) ),
                    e.mouseButton.button );
            }
            case sf::Event::EventType::MouseButtonReleased:
            {
                OnMouseButtonReleased(
                    m_modelToScreen.getInverse().transformPoint( static_cast< float >( e.mouseButton.x ), static_cast< float >( e.mouseButton.y ) ),
                    e.mouseButton.button );
            }
            case sf::Event::EventType::MouseMoved:
            {
                OnMouseMoved( m_modelToScreen.getInverse().transformPoint( static_cast< float >( e.mouseMove.x ), static_cast< float >( e.mouseMove.y ) ) );
            }
        }
    }

    void GenericGame::OnVideoSettingsChanged() { UpdateModelToScreen(); }

    void GenericGame::OnClose() { m_window.close(); }

    void GenericGame::OnKeyPressed( const sf::Event::KeyEvent& key ) {}

    void GenericGame::OnKeyReleased( const sf::Event::KeyEvent& key ) {}

    void GenericGame::OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button ) {}

    void GenericGame::OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button ) {}

    void GenericGame::OnMouseMoved( const sf::Vector2f& position ) {}

    sf::Vector2f GenericGame::GetMousePosition() const
    {
        return GetTransformation().getInverse().transformPoint(
            static_cast< float >( sf::Mouse::getPosition( m_window ).x ), static_cast< float >( sf::Mouse::getPosition( m_window ).y ) );
    }

    void GenericGame::UpdateModelToScreen()
    {
        auto w = static_cast< float >( m_window.getSize().x );
        auto h = static_cast< float >( m_window.getSize().y );
        ASSERT( w >= h, L"Vertical viewport is not supported" );

        m_modelToScreen = sf::Transform::Identity;
        m_modelToScreen.translate( { ( w - h ) / 2.0f, 0 } );
        m_modelToScreen.scale( { h, h } );

        m_scale = h;
    }
}