#include "game/graphics/Primitive.hpp"

#include "game/GenericGame.hpp"

#include <functional>

namespace Graphics
{
    Primitive::Primitive()
    {
    }

    Primitive::~Primitive()
    {
    }

    void Primitive::Render( sf::RenderTarget& target )
    {
        if( m_highlight )
        {
            target.draw( m_debugRect, Game::GenericGame::GetInstance()->GetToScreenTransform() );
        }
    }

    void Primitive::Init()
    {
        m_debugRect.setFillColor( sf::Color( 255, 0, 0, 128 ) );
        Position.AddCallback( [this]( const sf::Vector2f& newPosition ) { OnPositionChange( newPosition ); } );
        Size.AddCallback( [this]( const sf::Vector2f& newSize ) { OnSizeChange( newSize ); } );
    }

    void Primitive::OnPositionChange( const sf::Vector2f& newPosition )
    {
        m_debugRect.setPosition( newPosition );
    }

    void Primitive::OnSizeChange( const sf::Vector2f& newSize )
    {
        m_debugRect.setSize( newSize );
    }
}