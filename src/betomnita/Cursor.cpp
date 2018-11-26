#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"

namespace Betomnita
{
    Cursor::Cursor()
    {
        m_shape.SetPoints( {
            { 0.0f, 0.0f },
            { 0.02f, 0.012f },
            { 0.012f, 0.02f },
        } );
        m_shape.SetColor( sf::Color( 100, 100, 100, 255 ) );
    }

    Cursor::~Cursor()
    {
    }

    void Cursor::Render( sf::RenderTarget& target )
    {
        m_shape.Render( target );
    }
}