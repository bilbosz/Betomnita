#include "game/graphics/Text.hpp"

#include "app/Debug.hpp"

namespace Graphics
{
    Text::Text()
    {
        Init();
    }

    Text::~Text()
    {
    }

    void Text::Init()
    {
        Primitive::Init();
        String.AddCallback( [this]( const std::wstring& newString ) { OnStringChange( newString ); } );
        Font.AddCallback( [this]( const sf::Font& newFont ) { OnFontChange( newFont ); } );
    }

    void Text::Render( sf::RenderTarget& target )
    {
        Primitive::Render( target );
    }

    void Text::OnPositionChange( const sf::Vector2f& newPosition )
    {
        Primitive::OnPositionChange( newPosition );
    }

    void Text::OnSizeChange( const sf::Vector2f& newSize )
    {
        PrepareRender();
        Primitive::OnSizeChange( newSize );
    }

	void Text::OnStringChange( const std::wstring& newString )
    {
        PrepareRender();
    }

	void Text::OnFontChange( const sf::Font& newFont )
	{
		PrepareRender();
	}

    void Text::PrepareRender()
    {

    }
}
