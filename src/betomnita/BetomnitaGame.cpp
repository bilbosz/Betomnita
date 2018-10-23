#include "betomnita/BetomnitaGame.hpp"

#include "app/Debug.hpp"
#include "betomnita/Cursor.hpp"
#include "betomnita/state/MainMenuState.hpp"
#include "game/StateMachine.hpp"

namespace Betomnita
{
    BetomnitaGame* BetomnitaGame::s_instance = nullptr;

    BetomnitaGame::BetomnitaGame() : m_flowController( std::make_unique< Game::StateMachine >() )
    {
        ASSERT( !s_instance, L"There can be only one instance of Betomnita game" );
        s_instance = this;
    }

    BetomnitaGame::~BetomnitaGame()
    {
        ASSERT( s_instance, L"Betomnita game should exist before calling destructor" );
        s_instance = nullptr;
    }

    BetomnitaGame* BetomnitaGame::GetInstance()
    {
        ASSERT( s_instance, L"Betomnita game is not created yet" );
        return s_instance;
    }

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

        m_text.Font.Set( *Resource::DebugFont );
        m_text.LineHeight.Set( 0.05f );
        m_text.Position.Set( sf::Vector2f( 0.1f, 0.1f ) );
        m_text.SetHightlight( true );

        GenericGame::OnStart();
    };

    void BetomnitaGame::OnUpdate( sf::Time dt )
    {
        const auto& mousePosition = GetMousePosition();
        std::wostringstream out;
        out << L"(" << mousePosition.x << L", " << mousePosition.y << L")";
        m_text.String.Set( L"Jestem Adam" );
        m_flowController->OnUpdate( dt );

        GenericGame::OnUpdate( dt );
    }

    void BetomnitaGame::OnRender( sf::RenderTarget& target )
    {
        target.clear( { 0, 0, 0 } );
        target.draw( m_rect, GetToScreenTransform() );
        m_text.Render( target );
        m_window.display();

        GenericGame::OnRender( target );
    }

    void BetomnitaGame::OnVideoSettingsChanged()
    {
        GenericGame::OnVideoSettingsChanged();
    }

    void BetomnitaGame::OnClose()
    {
        GenericGame::OnClose();
    }

    void BetomnitaGame::OnKeyPressed( const sf::Event::KeyEvent& key )
    {
        GenericGame::OnKeyPressed( key );
    }

    void BetomnitaGame::OnKeyReleased( const sf::Event::KeyEvent& key )
    {
        GenericGame::OnKeyReleased( key );
    }

    void BetomnitaGame::OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button )
    {
        switch( button )
        {
            case sf::Mouse::Button::Left:
                m_text.LineHeight.Set( m_text.LineHeight.Get() + 0.01f );
                break;
            case sf::Mouse::Button::Right:
                m_text.LineHeight.Set( m_text.LineHeight.Get() - 0.01f );
                break;
            case sf::Mouse::Button::Middle:
                break;
        }
        GenericGame::OnMouseButtonPressed( position, button );
    }

    void BetomnitaGame::OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button )
    {
        GenericGame::OnMouseButtonReleased( position, button );
    }

    void BetomnitaGame::OnMouseMoved( const sf::Vector2f& position )
    {
        GenericGame::OnMouseMoved( position );
    }
}