#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"

namespace Betomnita
{
    Cursor::Cursor()
    {
        m_top.SetPoints( { { 0.0f, 0.0f }, { 0.098997996f, 0.04f }, { 0.049925796f, 0.04f } } );
        m_bottom.SetPoints( { { 0.0f, 0.0f }, { 0.044378496f, 0.05f }, { 0.044378496f, 0.1f } } );
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