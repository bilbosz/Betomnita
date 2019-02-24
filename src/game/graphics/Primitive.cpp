#include "game/graphics/Primitive.hpp"

#include "game/GenericGame.hpp"

#include <functional>

namespace Game::Graphics
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
            sf::Transform resultTransform = Game::GenericGame::GetInstance()->GetToScreenTransform();
            resultTransform.combine( m_transform );
            target.draw( m_debugRect, resultTransform );
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

    void Primitive::OnTransformChange()
    {
    }

    void Primitive::OnPivotChange()
    {
#ifdef DEBUG
        m_debugRect.setOrigin( m_pivot );
#endif
    }

    void Primitive::OnSizeChange()
    {
#ifdef DEBUG
        m_debugRect.setSize( m_size );
#endif
    }
}