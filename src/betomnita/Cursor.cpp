#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"
#include "betomnita/resources/Resources.hpp"
#include "game/graphics/Polygon.hpp"

namespace Betomnita
{
    Cursor::Cursor() : m_shape( std::make_unique< Game::Graphics::Polygon >() )
    {
        m_shape->LoadFromFile( Resources::CursorPath );
    }

    Cursor::~Cursor()
    {
    }

    void Cursor::Render( sf::RenderTarget& target )
    {
        m_shape->Render( target );
    }

    void Cursor::SetPosition( const sf::Vector2f& value )
    {
        m_position = value;
        m_shape->SetPosition( m_position );
    }
}