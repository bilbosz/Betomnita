#include "MainMenuLayout.hpp"

#include "app/Application.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "game/graphics/Text.hpp"
#include "resource/Resource.hpp"

#include <functional>

namespace Betomnita::Layout
{
    MainMenuLayout::MainMenuLayout() : m_title( std::make_unique< Game::Graphics::Text >() )
    {
        m_title->SetColor( sf::Color( 100, 100, 100, 255 ) );
        m_title->SetString( Resource::GameName );
        m_title->SetFont( *Resource::DefaultFont );
        m_title->SetPosition( { 0.1f, 0.1f } );
        m_title->SetLineHeight( 0.1f );

        m_options.push_back( { EntryId::NewGame, L"New Game", std::make_unique< Game::Graphics::Text >(), []() { MESSAGE( L"New Game Requested" ); } } );
        m_options.push_back( { EntryId::Exit, L"Quit Game", std::make_unique< Game::Graphics::Text >(), []() { App::Application::GetInstance()->RequestShutdown(); } } );
        auto y = 0.25f;
        auto x = 0.1f;
        for( auto& option : m_options )
        {
            option.Control->SetColor( sf::Color( 150, 150, 150, 255 ) );
            option.Control->SetString( option.Text );
            option.Control->SetFont( *Resource::DefaultFont );
            option.Control->SetLineHeight( 0.05f );
            option.Control->SetPosition( { x, y } );
            y += 1.25f * option.Control->GetLineHeight();
        }
    }

    MainMenuLayout::~MainMenuLayout()
    {
    }

    void MainMenuLayout::Show()
    {
    }

    void MainMenuLayout::Hide()
    {
    }

    void MainMenuLayout::OnRender( sf::RenderTarget& target )
    {
        m_title->Render( target );
        for( const auto& option : m_options )
        {
            option.Control->Render( target );
        }
    }

    void MainMenuLayout::OnUpdate( const sf::Time& dt )
    {
    }
}
