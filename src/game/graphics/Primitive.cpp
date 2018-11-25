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
#ifdef DEBUG
        if( m_highlight )
        {
            target.draw( m_debugRect, Game::GenericGame::GetInstance()->GetToScreenTransform() );
        }
#endif
    }

    void Primitive::Init()
    {
#ifdef DEBUG
        m_debugRect.setFillColor( sf::Color( 255, 0, 0, 128 ) );
#endif
    }

    void Primitive::OnPositionChange()
    {
#ifdef DEBUG
        m_debugRect.setPosition( m_position );
#endif
    }

    void Primitive::OnSizeChange()
    {
#ifdef DEBUG
        m_debugRect.setSize( m_size );
#endif
    }
}