#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"

namespace Betomnita
{
    Cursor::Cursor()
    {
        m_top.SetPoints( { { 0.0f, 0.0f }, { 0.024749499f * 1.3f, 0.01f * 1.3f }, { 0.012481449f * 1.3f, 0.01f * 1.3f } } );
        m_bottom.SetPoints( { { 0.0f, 0.0f }, { 0.011094624f * 1.3f, 0.0125f * 1.3f }, { 0.011094624f * 1.3f, 0.025f * 1.3f } } );
    }

    Cursor::~Cursor()
    {
    }

    void Cursor::Render( sf::RenderTarget& target )
    {
        m_top.Render( target );
        m_bottom.Render( target );
    }
}