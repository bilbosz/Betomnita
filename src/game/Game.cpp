#include "game/Game.hpp"

#include "app/Error.hpp"

#include <SFML/Graphics.hpp>

namespace Game
{
    Game::Game::Game() {}

    Game::Game::~Game() {}

    void Game::Game::OnStart()
    {
        m_circle.setRadius( 0.03f );
        m_circle.setOrigin( { 0.03f, 0.03f } );
        m_circle.setFillColor( sf::Color::Red );
    }

    void Game::Game::OnUpdate( sf::Time dt ) {}

    void Game::Game::OnRender( sf::Time dt )
    {
        m_text.setFont( *Resource::debugFont );
        m_text.setCharacterSize( 17 );
        m_window.clear( { 50, 50, 50 } );
        m_window.draw( m_text );
        m_window.draw( m_circle, m_modelToScreen );
        m_window.display();
    }

    void Game::Game::OnEvent( const sf::Event& e ) {}

    void Game::Game::OnVideoSettingsChanged() { UpdateModelToScreen(); }

    void Game::Game::OnClose() {}

    void Game::Game::OnKeyPressed( const sf::Event::KeyEvent& key ) {}

    void Game::Game::OnKeyReleased( const sf::Event::KeyEvent& key ) {}

    void Game::Game::OnMouseButtonPressed( const sf::Event::MouseButtonEvent& mouseButton )
    {
        auto logicalPoint = m_modelToScreen.getInverse().transformPoint( static_cast< float >( mouseButton.x ), static_cast< float >( mouseButton.y ) );
        MESSAGE( L"(" << logicalPoint.x << L", " << logicalPoint.y << L")" );
    }

    void Game::Game::OnMouseButtonReleased( const sf::Event::MouseButtonEvent& mouseButton ) {}

    void Game::Game::OnMouseMoved( const sf::Event::MouseMoveEvent& mouseMove )
    {
        auto logicalPoint = m_modelToScreen.getInverse().transformPoint( static_cast< float >( mouseMove.x ), static_cast< float >( mouseMove.y ) );
        m_circle.setPosition( logicalPoint );
        std::wostringstream out;
        out << L"(" << logicalPoint.x << L", " << logicalPoint.y << L")";
        m_text.setString( out.str() );
    }

    void Game::Game::UpdateModelToScreen()
    {
        auto w = static_cast< float >( m_window.getSize().x );
        auto h = static_cast< float >( m_window.getSize().y );
        ASSERT( w >= h, L"Vertical viewport is not supported" );

        auto modelW = w / h;
        auto modelH = 1.0f;

        m_modelToScreen = sf::Transform::Identity;
        m_modelToScreen.translate( { -( modelW - modelH ) / 2.0f, 0 } );
        m_modelToScreen.scale( { h, h } );
    }
}