#include "app/Debug.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/state/MainMenuState.hpp"

namespace Betomnita
{
    BetomnitaGame::BetomnitaGame() {}

    BetomnitaGame::~BetomnitaGame() {}

    void BetomnitaGame::OnStart()
    {
        m_flowController.RegisterState( 0, std::make_shared< MainMenuState >() );
        m_flowController.PushState( 0 );

        m_circle.setRadius( 0.01f );
        m_circle.setOrigin( { 0.01f, 0.01f } );
        m_circle.setFillColor( sf::Color::Red );

        m_rect.setPosition( { 0.0f, 0.0f } );
        m_rect.setSize( { 1.0f, 1.0f } );
        m_rect.setOutlineThickness( 0.01f );
        m_rect.setFillColor( sf::Color::Transparent );
        m_rect.setOutlineColor( sf::Color::Blue );

        m_text.setFont( *Resource::DebugFont );
        m_text.setCharacterSize( 17 );
        m_text.setPosition( 0.5f, 0.5f );

        GenericGame::OnStart();
    };

    void BetomnitaGame::OnUpdate( sf::Time dt )
    {
        const auto& mousePosition = GetMousePosition();
        m_circle.setPosition( mousePosition );
        std::wostringstream out;
        out << L"(" << mousePosition.x << L", " << mousePosition.y << L")";
        m_text.setString( out.str() );
        m_flowController.OnUpdate( dt );

        GenericGame::OnUpdate( dt );
    }

    void BetomnitaGame::OnRender( sf::Time dt )
    {
		const auto oldPos = m_text.getPosition();
		const auto newPos = GetTransformation().transformPoint( oldPos );
		m_text.setPosition( newPos );

        m_window.clear( { 50, 50, 50 } );
        m_window.draw( m_text );
        m_window.draw( m_rect, GetTransformation() );
        m_window.draw( m_circle, GetTransformation() );
        m_window.display();

		m_text.setPosition( oldPos );

        GenericGame::OnRender( dt );
    }

    void BetomnitaGame::OnVideoSettingsChanged() { GenericGame::OnVideoSettingsChanged(); }

    void BetomnitaGame::OnClose() { GenericGame::OnClose(); }

    void BetomnitaGame::OnKeyPressed( const sf::Event::KeyEvent& key ) { GenericGame::OnKeyPressed( key ); }

    void BetomnitaGame::OnKeyReleased( const sf::Event::KeyEvent& key ) { GenericGame::OnKeyReleased( key ); }

    void BetomnitaGame::OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button )
    {
        MESSAGE( static_cast< int >( button ) << L"(" << position.x << L", " << position.y << L")" );
        GenericGame::OnMouseButtonPressed( position, button );
    }

    void BetomnitaGame::OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button ) { GenericGame::OnMouseButtonReleased( position, button ); }

    void BetomnitaGame::OnMouseMoved( const sf::Vector2f& position ) { GenericGame::OnMouseMoved( position ); }
}