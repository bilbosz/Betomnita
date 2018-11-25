#include "game/GenericGame.hpp"

#include "app/Debug.hpp"

#include <SFML/Graphics.hpp>

namespace Game
{
    GenericGame* GenericGame::s_instance = nullptr;

    GenericGame::GenericGame()
    {
        ASSERT( !s_instance, L"There can be only one instance of generic game" );
        s_instance = this;
    }

    GenericGame::~GenericGame()
    {
        ASSERT( s_instance, L"Generic game should exist before calling destructor" );
        s_instance = nullptr;
    }

    GenericGame* GenericGame::GetInstance()
    {
        ASSERT( s_instance, L"Generic game object is not created yet" );
        return s_instance;
    }

    sf::Vector2f GenericGame::GetToScreenScale() const
    {
        const auto& matrix = m_modelToScreen.getMatrix();
        for( size_t y = 0; y < 4; ++y )
        {
            for( size_t x = 0; x < 4; ++x )
            {
                ASSERT(
                    y == 0 && x == 0 || y == 1 && x == 1 || y == 2 && x == 2 || y == 3 && x == 0 || matrix[ y * 4 + x ] == 1.0f || matrix[ y * 4 + x ] == 0.0f,
                    L"Only transformations available in model to pixel are x translation and x and y scaling" );
            }
        }
        ASSERT( matrix[ 0 * 4 + 0 ] == matrix[ 1 * 4 + 1 ], L"x and y scale factors should be equal" );
        return { matrix[ 0 * 4 + 0 ], matrix[ 1 * 4 + 1 ] };
    }

    sf::Vector2f GenericGame::GetToModelScale() const
    {
        const auto& matrix = m_screenToModel.getMatrix();
        for( size_t y = 0; y < 4; ++y )
        {
            for( size_t x = 0; x < 4; ++x )
            {
                ASSERT(
                    y == 0 && x == 0 || y == 1 && x == 1 || y == 2 && x == 2 || y == 3 && x == 0 || matrix[ y * 4 + x ] == 1.0f || matrix[ y * 4 + x ] == 0.0f,
                    L"Only transformations available in model to pixel are x translation and x and y scaling" );
            }
        }
        ASSERT( matrix[ 0 * 4 + 0 ] == matrix[ 1 * 4 + 1 ], L"x and y scale factors should be equal" );
        return { matrix[ 0 * 4 + 0 ], matrix[ 1 * 4 + 1 ] };
    }

    sf::Vector2f GenericGame::GetMousePosition() const
    {
        return GetToModelTransform().transformPoint(
            static_cast< float >( sf::Mouse::getPosition( m_window ).x ), static_cast< float >( sf::Mouse::getPosition( m_window ).y ) );
    }

    void GenericGame::OnStart()
    {
    }

    void GenericGame::OnUpdate( const sf::Time& dt )
    {
        m_gameTime += dt;
    }

    void GenericGame::OnRender( sf::RenderTarget& target )
    {
    }

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
            break;
            case sf::Event::EventType::MouseButtonPressed:
            {
                OnMouseButtonPressed(
                    m_modelToScreen.getInverse().transformPoint( static_cast< float >( e.mouseButton.x ), static_cast< float >( e.mouseButton.y ) ),
                    e.mouseButton.button );
            }
            break;
            case sf::Event::EventType::MouseButtonReleased:
            {
                OnMouseButtonReleased(
                    m_modelToScreen.getInverse().transformPoint( static_cast< float >( e.mouseButton.x ), static_cast< float >( e.mouseButton.y ) ),
                    e.mouseButton.button );
            }
            break;
            case sf::Event::EventType::MouseMoved:
            {
                OnMouseMoved( m_modelToScreen.getInverse().transformPoint( static_cast< float >( e.mouseMove.x ), static_cast< float >( e.mouseMove.y ) ) );
            }
            break;
        }
    }

    void GenericGame::OnVideoSettingsChanged()
    {
        UpdateModelToScreen();
    }

    void GenericGame::OnScreenshotTaken( const std::string& path )
    {
    }

    void GenericGame::OnClose()
    {
        m_window.close();
    }

    void GenericGame::OnKeyPressed( const sf::Event::KeyEvent& key )
    {
    }

    void GenericGame::OnKeyReleased( const sf::Event::KeyEvent& key )
    {
    }

    void GenericGame::OnMouseButtonPressed( const sf::Vector2f& position, sf::Mouse::Button button )
    {
    }

    void GenericGame::OnMouseButtonReleased( const sf::Vector2f& position, sf::Mouse::Button button )
    {
    }

    void GenericGame::OnMouseMoved( const sf::Vector2f& position )
    {
    }

    void GenericGame::UpdateModelToScreen()
    {
        auto w = static_cast< float >( m_window.getSize().x );
        auto h = static_cast< float >( m_window.getSize().y );
        ASSERT( w >= h, L"Vertical viewport is not supported" );

        m_modelToScreen = sf::Transform::Identity;
        m_modelToScreen.translate( { ( w - h ) / 2.0f, 0 } );
        m_modelToScreen.scale( { h, h } );

        m_screenToModel = m_modelToScreen.getInverse();
    }
}