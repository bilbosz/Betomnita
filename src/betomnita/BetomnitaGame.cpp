#include "betomnita/BetomnitaGame.hpp"

#include "app/Debug.hpp"
#include "betomnita/Cursor.hpp"
#include "betomnita/state/MainMenuState.hpp"
#include "game/StateMachine.hpp"

#include <sstream>

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

        m_text.SetFont( *Resource::DefaultFont );
        m_text.SetPosition( { 0.0f, 0.0f } );
        m_text.SetLineHeight( 0.1f );
        m_text.SetHightlight( false );

        m_path.setPrimitiveType( sf::LineStrip );

        GenericGame::OnStart();
    };

    void BetomnitaGame::OnUpdate( sf::Time dt )
    {
        const auto& mousePosition = GetMousePosition();
        std::wostringstream out;
        out << L"(" << mousePosition.x << L", " << mousePosition.y << L")";
        m_text.SetString( out.str() );
        m_flowController->OnUpdate( dt );

        GenericGame::OnUpdate( dt );
    }

    void BetomnitaGame::OnRender( sf::RenderTarget& target )
    {
        target.clear( { 128, 180, 180 } );
        m_polygon.Render( target );
        target.draw( m_path, GetToScreenTransform() );
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
        switch( key.code )
        {
            case sf::Keyboard::Key::Delete:
            {
                m_points.clear();
                m_polygon.SetPoints( m_points );
                m_path.clear();
            }
            break;
            case sf::Keyboard::Key::Enter:
            {
                auto error = Graphics::Polygon::GetPointsErrors( m_points );
                if( !error.has_value() )
                {
                    m_polygon.SetPoints( m_points );
                }
            }
            break;
            case sf::Keyboard::Key::Escape:
            {
                RequestShutdown();
            }
            break;
        }
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
                if( m_points.empty() || !m_points.empty() && m_points.back() != position )
                {
                    m_points.emplace_back( position );
                    m_path.append( { position, sf::Color::White } );
                }
                break;
            case sf::Mouse::Button::Right:
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
        if( sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) )
        {
            if( m_points.empty() || !m_points.empty() && m_points.back() != position )
            {
                m_points.emplace_back( position );
                m_path.append( { position, sf::Color::White } );
            }
        }
        GenericGame::OnMouseMoved( position );
    }
}