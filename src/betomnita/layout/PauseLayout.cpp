#include "PauseLayout.hpp"

#include "app/Application.hpp"
#include "betomnita/BetomnitaGame.hpp"
#include "betomnita/event/EventRegistration.hpp"
#include "betomnita/state/PauseState.hpp"
#include "game/graphics/Polygon.hpp"
#include "game/graphics/Text.hpp"
#include "resource/Resource.hpp"

#include <functional>

namespace Betomnita::Layout
{
    PauseLayout::PauseLayout( PauseState* state )
        : m_shroud( std::make_unique< Game::Graphics::Polygon >() ), m_title( std::make_unique< Game::Graphics::Text >() ), m_state( state )
    {
        const auto& modelAABB = Betomnita::BetomnitaGame::GetInstance()->GetModelAABB();
        m_shroud->SetColor( Resource::Shroud );
        m_shroud->SetPoints( { { modelAABB.left, modelAABB.top },
                               { modelAABB.left + modelAABB.width, modelAABB.top },
                               { modelAABB.left + modelAABB.width, modelAABB.top + modelAABB.height },
                               { modelAABB.left, modelAABB.top + modelAABB.height } } );

        m_title->SetColor( Resource::MenuTitle );
        m_title->SetString( L"Pause" );
        m_title->SetFont( *Resource::DefaultFont );
        m_title->SetPosition( { 0.1f, 0.1f } );
        m_title->SetLineHeight( 0.1f );

        m_options.push_back( { EntryId::Resume, L"Resume", std::make_unique< Game::Graphics::Text >(), [this]() { m_state->OnResumeRequest(); } } );
        m_options.push_back( { EntryId::MainMenu, L"Main Menu", std::make_unique< Game::Graphics::Text >(), [this]() { m_state->OnMainMenuRequest(); } } );
        m_options.push_back( { EntryId::ExitGame, L"Exit Game", std::make_unique< Game::Graphics::Text >(), [this]() { m_state->OnExitGameRequest(); } } );
        auto y = 0.25f;
        auto x = 0.1f;
        for( auto& option : m_options )
        {
            option.Control->SetColor( Resource::MenuEntryNormal );
            option.Control->SetString( option.Text );
            option.Control->SetFont( *Resource::DefaultFont );
            option.Control->SetLineHeight( 0.05f );
            option.Control->SetPosition( { x, y } );
            y += 1.25f * option.Control->GetLineHeight();
        }

        Game::EventSystem::Event< Resource::EventId::OnMouseButtonPressed >::AddListener(
            { Resource::ListenerId::PauseMenuClick, false, [this]( const sf::Vector2f& pos, sf::Mouse::Button btn ) {
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

        Game::EventSystem::Event< Resource::EventId::OnMouseMoved >::AddListener(
            { Resource::ListenerId::PauseMenuHover, false, [this]( const sf::Vector2f& pos ) {
                 for( auto& option : m_options )
                 {
                     auto& ctrl = option.Control;
                     auto aabb = sf::FloatRect( ctrl->GetPosition(), ctrl->GetSize() );
                     option.Control->SetColor( aabb.contains( pos ) ? Resource::MenuEntrySelected : Resource::MenuEntryNormal );
                 }
             } } );
    }

    PauseLayout::~PauseLayout()
    {
    }

    void PauseLayout::Show()
    {
        const auto& pos = Betomnita::BetomnitaGame::GetInstance()->GetMousePosition();
        for( auto& option : m_options )
        {
            auto& ctrl = option.Control;
            auto aabb = sf::FloatRect( ctrl->GetPosition(), ctrl->GetSize() );
            option.Control->SetColor( aabb.contains( pos ) ? Resource::MenuEntrySelected : Resource::MenuEntryNormal );
        }
        Game::EventSystem::Event< Resource::EventId::OnMouseButtonPressed >::GetListener( Resource::ListenerId::PauseMenuClick ).IsEnabled = true;
        Game::EventSystem::Event< Resource::EventId::OnMouseMoved >::GetListener( Resource::ListenerId::PauseMenuHover ).IsEnabled = true;
    }

    void PauseLayout::Hide()
    {
        Game::EventSystem::Event< Resource::EventId::OnMouseButtonPressed >::GetListener( Resource::ListenerId::PauseMenuClick ).IsEnabled = false;
        Game::EventSystem::Event< Resource::EventId::OnMouseMoved >::GetListener( Resource::ListenerId::PauseMenuHover ).IsEnabled = false;
        for( auto& option : m_options )
        {
            option.Control->SetColor( Resource::MenuEntryNormal );
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
