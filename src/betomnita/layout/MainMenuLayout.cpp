#include "MainMenuLayout.hpp"

#include "app/Application.hpp"
#include "game/graphics/Text.hpp"
#include "resource/Resource.hpp"

namespace Betomnita::Layout
{
    MainMenuLayout::MainMenuLayout() : m_title( std::make_unique< Graphics::Text >() )
    {
        m_title->SetString( Resource::WindowTitle );
        m_title->SetFont( *Resource::DefaultFont );
        m_title->SetPosition( { 0.0f, 0.1f } );
        m_title->SetLineHeight( 0.1f );
        m_title->SetColor( sf::Color::White );

        m_options.push_back( { EntryId::NewGame, L"New Game", []() { MESSAGE( L"New Game Requested" ); } } );
        m_options.push_back( { EntryId::Exit, L"Quit Game", []() { App::Application::GetInstance()->RequestShutdown(); } } );
    }

    MainMenuLayout::~MainMenuLayout()
    {
    }

    void MainMenuLayout::Show()
    {
        
    }

    void MainMenuLayout::OnRender( sf::RenderTarget& target )
    {
        m_title->Render( target );
    }
}
