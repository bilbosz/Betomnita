#include "betomnita/BetomnitaGame.hpp"

#include "app/Debug.hpp"
#include "betomnita/Cursor.hpp"
#include "betomnita/state/MainMenuState.hpp"
#include "game/StateMachine.hpp"

namespace Betomnita
{
    BetomnitaGame::BetomnitaGame() : m_flowController( std::make_unique< Game::StateMachine >() ), m_cursor( std::make_unique< Cursor >() ) {}

    BetomnitaGame::~BetomnitaGame() {}

    void BetomnitaGame::OnStart()
    {
        m_flowController->RegisterState( 0, std::make_shared< MainMenuState >() );
        m_flowController->PushState( 0 );

        m_circle.setRadius( 0.01f );
        m_circle.setOrigin( { 0.01f, 0.01f } );
        m_circle.setFillColor( sf::Color::Red );

        m_rect.setPosition( { 0.0f, 0.0f } );
        m_rect.setSize( { 1.0f, 1.0f } );
        m_rect.setOutlineThickness( 0.01f );
        m_rect.setFillColor( sf::Color::Transparent );
        m_rect.setOutlineColor( sf::Color::Blue );

        m_text.setFont( *Resource::DebugFont );
        m_text.setFillColor( sf::Color::Black );
        m_text.setCharacterSize( 17 );
        m_text.setPosition( 0.5f, 0.5f );

        GenericGame::OnStart();
    };

    void BetomnitaGame::OnUpdate( sf::Time dt )
    {
        const auto& mousePosition = GetMousePosition();
        m_cursor->SetPosition( mousePosition );
        std::wostringstream out;
        out << L"(" << mousePosition.x << L", " << mousePosition.y << L")";
        m_text.setString( out.str() );
        m_flowController->OnUpdate( dt );

        GenericGame::OnUpdate( dt );
    }

    void BetomnitaGame::OnRender( sf::RenderTarget& target )
    {
        const auto oldPos = m_text.getPosition();
        const auto newPos = GetTransformation().transformPoint( oldPos );
        m_text.setPosition( newPos );

        target.clear( { 225, 225, 225 } );
        target.draw( m_text );
        target.draw( m_rect, GetTransformation() );
        target.draw( m_circle, GetTransformation() );
        m_cursor->OnRender( target, sf::Transform::Identity );
        m_window.display();

        m_text.setPosition( oldPos );

        GenericGame::OnRender( target );
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