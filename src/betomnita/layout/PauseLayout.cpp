#include "PauseLayout.hpp"

#include "app/Application.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/resources/Resources.hpp"
#include "betomnita/state/PauseState.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/Text.hpp"

#include <functional>

namespace Betomnita::Layouts
{
    PauseLayout::PauseLayout( States::PauseState* state )
        : m_shroud( std::make_unique< Game::Graphics::Polygon >() ), m_title( std::make_unique< Game::Graphics::Text >() ), m_state( state )
    {
        const auto& modelAABB = BetomnitaGame::GetInstance()->GetModelAABB();
        m_shroud->SetColor( Resources::Shroud );
        m_shroud->SetPoints( {
            { modelAABB.left, modelAABB.top + modelAABB.height },
            { modelAABB.left + modelAABB.width, modelAABB.top + modelAABB.height },
            { modelAABB.left + modelAABB.width, modelAABB.top },
            { modelAABB.left, modelAABB.top },
        } );

        m_title->SetColor( Resources::MenuTitle );
        m_title->SetString( L"Pause" );
        m_title->SetFont( *Resources::DefaultFont );
        m_title->SetPosition( { 0.1f, 0.1f } );
        m_title->SetLineHeight( 0.1f );

        m_options.push_back( { EntryId::Resume, L"Resume", std::make_unique< Game::Graphics::Text >(), [this]() { m_state->OnResumeRequest(); } } );
        m_options.push_back( { EntryId::MainMenu, L"Main Menu", std::make_unique< Game::Graphics::Text >(), [this]() { m_state->OnMainMenuRequest(); } } );
        m_options.push_back( { EntryId::ExitGame, L"Exit Game", std::make_unique< Game::Graphics::Text >(), [this]() { m_state->OnExitGameRequest(); } } );
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
            { Resources::ListenerId::PauseMenuClick, false, [this]( const sf::Vector2f& pos, sf::Mouse::Button btn ) {
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
            { Resources::ListenerId::PauseMenuHover, false, [this]( const sf::Vector2f& pos ) {
                 for( auto& option : m_options )
                 {
                     auto& ctrl = option.Control;
                     auto aabb = sf::FloatRect( ctrl->GetPosition(), ctrl->GetSize() );
                     option.Control->SetColor( aabb.contains( pos ) ? Resources::MenuEntrySelected : Resources::MenuEntryNormal );
                 }
             } } );

        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::AddListener(
            { Resources::ListenerId::PauseMenuLeave, false, [this]( const sf::Event::KeyEvent& key ) {
                 if( key.code == sf::Keyboard::Escape )
                 {
                     m_state->OnResumeRequest();
                 }
             } } );
    }

    PauseLayout::~PauseLayout()
    {
    }

    void PauseLayout::Show()
    {
        const auto& pos = BetomnitaGame::GetInstance()->GetMousePosition();
        for( auto& option : m_options )
        {
            auto& ctrl = option.Control;
            auto aabb = sf::FloatRect( ctrl->GetPosition(), ctrl->GetSize() );
            option.Control->SetColor( aabb.contains( pos ) ? Resources::MenuEntrySelected : Resources::MenuEntryNormal );
        }
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::EnableListener( Resources::ListenerId::PauseMenuClick );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::EnableListener( Resources::ListenerId::PauseMenuHover );
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::EnableListener( Resources::ListenerId::PauseMenuLeave );
    }

    void PauseLayout::Hide()
    {
        Game::EventSystem::Event< Resources::EventId::OnMouseButtonPressed >::DisableListener( Resources::ListenerId::PauseMenuClick );
        Game::EventSystem::Event< Resources::EventId::OnMouseMoved >::DisableListener( Resources::ListenerId::PauseMenuHover );
        Game::EventSystem::Event< Resources::EventId::OnKeyPressed >::DisableListener( Resources::ListenerId::PauseMenuLeave );
        for( auto& option : m_options )
        {
            option.Control->SetColor( Resources::MenuEntryNormal );
        }
    }

    void PauseLayout::OnRender( sf::RenderTarget& target )
    {
        m_shroud->Render( target );
        m_title->Render( target );
        for( const auto& option : m_options )
        {
            option.Control->Render( target );
        }
    }

    void PauseLayout::OnUpdate( const sf::Time& dt )
    {
    }
}
