#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"

namespace Betomnita
{
    Cursor::Cursor()
    {
        m_top.SetPoints( { { 0.0f, 0.0f }, { 0.01f, 0.0f }, { 0.01f, 0.01f } } );
        //m_bottom.SetPoints( { { 0.0f, -0.005f }, { 0.0f, 0.0f }, { 0.0044378496f, 0.01f } } );
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