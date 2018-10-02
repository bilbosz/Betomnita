#include "game/GenericGame.hpp"

#include "app/Error.hpp"

#include <SFML/Graphics.hpp>

namespace Game
{
    GenericGame::GenericGame() {}

    GenericGame::~GenericGame() {}

    void GenericGame::OnStart()
    {
        m_circle.setRadius( 0.01f );
        m_circle.setOrigin( { 0.01f, 0.01f } );
        m_circle.setFillColor( sf::Color::Red );
        m_text.setPosition( 0.5f, 0.5f );
    }

    void GenericGame::OnUpdate( sf::Time dt )
    {
        auto logicalPoint = m_modelToScreen.getInverse().transformPoint(
            static_cast< float >( sf::Mouse::getPosition( m_window ).x ), static_cast< float >( sf::Mouse::getPosition( m_window ).y ) );
        m_circle.setPosition( logicalPoint );
        std::wostringstream out;
        out << L"(" << logicalPoint.x << L", " << logicalPoint.y << L")";
        m_text.setString( out.str() );
    }

    void GenericGame::OnRender( sf::Time dt )
    {
        m_text.setFont( *Resource::DebugFont );
        m_text.setCharacterSize( 17 );
        auto textOldPos = m_text.getPosition();
        m_text.setPosition( m_modelToScreen.transformPoint( textOldPos ) );
        m_window.clear( { 50, 50, 50 } );
        m_window.draw( m_text );
        m_window.draw( m_circle, m_modelToScreen );
        m_window.display();
        m_text.setPosition( textOldPos );
    }

    void GenericGame::OnEvent( const sf::Event& e ) {}

    void GenericGame::OnVideoSettingsChanged() { UpdateModelToScreen(); }

    void GenericGame::OnClose() { m_window.close(); }

    void GenericGame::OnKeyPressed( const sf::Event::KeyEvent& key )
    {
        switch( key.code )
        {
            case sf::Keyboard::Key::Escape:
            {
                RequestShutdown();
            }
            break;
        }
    }

    void GenericGame::OnKeyReleased( const sf::Event::KeyEvent& key ) {}

    void GenericGame::OnMouseButtonPressed( const sf::Event::MouseButtonEvent& mouseButton )
    {
        auto logicalPoint = m_modelToScreen.getInverse().transformPoint( static_cast< float >( mouseButton.x ), static_cast< float >( mouseButton.y ) );
        MESSAGE( L"(" << logicalPoint.x << L", " << logicalPoint.y << L")" );
    }

    void GenericGame::OnMouseButtonReleased( const sf::Event::MouseButtonEvent& mouseButton ) {}

    void GenericGame::OnMouseMoved( const sf::Event::MouseMoveEvent& mouseMove ) {}

    void GenericGame::UpdateModelToScreen()
    {
        auto w = static_cast< float >( m_window.getSize().x );
        auto h = static_cast< float >( m_window.getSize().y );
        ASSERT( w >= h, L"Vertical viewport is not supported" );

        m_modelToScreen = sf::Transform::Identity;
        m_modelToScreen.translate( { ( w - h ) / 2.0f, 0 } );
        m_modelToScreen.scale( { h, h } );
    }
}