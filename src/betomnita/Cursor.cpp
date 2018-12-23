#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"
#include "betomnita/resources/Resources.hpp"

namespace Betomnita
{
    Cursor::Cursor()
    {
        m_shape.LoadFromFile( Resources::CursorPath, 0.0004f );
    }

    Cursor::~Cursor()
    {
    }

    void Cursor::Render( sf::RenderTarget& target )
    {
        m_shape.Render( target );
    }
}