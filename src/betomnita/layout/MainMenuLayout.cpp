#include "MainMenuLayout.hpp"

#include "app/Application.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/resources/Resources.hpp"
#include "betomnita/state/MainMenuState.hpp"
#include "game/graphics/Text.hpp"

#include <functional>

namespace Betomnita::Layouts
{
    MainMenuLayout::MainMenuLayout( States::MainMenuState* state ) : m_title( std::make_unique< Game::Graphics::Text >() ), m_state( state )
    {
        m_title->SetColor( Resources::MenuTitle );
        m_title->SetString( Resources::GameName );
        m_title->SetFont( *Resources::DefaultFont );
        m_title->SetPosition( { 0.1f, 0.1f } );
        m_title->SetLineHeight( 0.1f );

        m_options.push_back( { EntryId::NewGame, L"New Game", std::make_unique< Game::Graphics::Text >(), [this]() { m_state->OnNewGameRequest(); } } );
        m_options.push_back( { EntryId::Exit, L"Exit Game", std::make_unique< Game::Graphics::Text >(), [this]() { m_state->OnExitRequest(); } } );
        auto y = 0.25f;
        auto x = 0.1f;
        for( auto& option : m_options )
        {
            option.Control->SetColor( Resources::MenuEntryNormal );
            option.Control->SetString( option.Text );
            option.Control->SetFont( *Resources::DefaultFont );
            option.Control->SetLineHeight( 0.05f );
            option.Control->SetPosition( { x, y } );
            y += 1.25f * option.Control->GetLineHeight();
        }

        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::AddListener(
            { Resources::ListenerId::MainMenuClick, false, [this]( const sf::Vector2f& pos, sf::Mouse::Button btn ) {
                 if( btn == sf::Mouse::Button::Left )
                 {
                     for( auto& option : m_options )
                     {
                         auto& ctrl = option.Control;
                         auto aabb = sf::FloatRect( ctrl->GetPosition(), ctrl->GetSize() );
                         if( aabb.contains( pos ) )
                         {
                             option.OnEnter();
                         }
                     }
                 }
             } } );

        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::AddListener(
            { Resources::ListenerId::MainMenuHover, false, [this]( const sf::Vector2f& pos ) {
                 for( auto& option : m_options )
                 {
                     auto& ctrl = option.Control;
                     auto aabb = sf::FloatRect( ctrl->GetPosition(), ctrl->GetSize() );
                     option.Control->SetColor( aabb.contains( pos ) ? Resources::MenuEntrySelected : Resources::MenuEntryNormal );
                 }
             } } );
    }

    MainMenuLayout::~MainMenuLayout()
    {
    }

    void MainMenuLayout::Show()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::EnableListener( Resources::ListenerId::MainMenuClick );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::EnableListener( Resources::ListenerId::MainMenuHover );
    }

    void MainMenuLayout::Hide()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::DisableListener( Resources::ListenerId::MainMenuClick );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::DisableListener( Resources::ListenerId::MainMenuHover );
        for( auto& option : m_options )
        {
            option.Control->SetColor( Resources::MenuEntryNormal );
        }
    }

    void MainMenuLayout::OnRender( sf::RenderTarget& target )
    {
        target.clear( Resources::MenuBackground );
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
