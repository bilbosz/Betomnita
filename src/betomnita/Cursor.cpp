#include "betomnita/Cursor.hpp"

#include "app/Debug.hpp"

namespace Betomnita
{
    Cursor::Cursor()
    {
        auto texture = sf::Texture();
        VERIFY( texture.loadFromFile( "res/cursor/regular.png" ) );
        m_asset.setTexture( texture, true );
    }

    Cursor::~Cursor()
    {
    }

    void Cursor::OnRender( sf::RenderTarget& target, const sf::Transform& transformation )
    {
        target.draw( m_asset, transformation );
    }
}