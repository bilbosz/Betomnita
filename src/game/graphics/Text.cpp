#include "game/graphics/Text.hpp"

#include "app/Debug.hpp"
#include "game/GenericGame.hpp"

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
        target.draw( m_text );
        Primitive::Render( target );
    }

    void Text::OnPositionChange( const sf::Vector2f& newPosition )
    {
		Update();
        Primitive::OnPositionChange( newPosition );
    }

    void Text::OnSizeChange( const sf::Vector2f& newSize )
    {
        Update();
        Primitive::OnSizeChange( newSize );
    }

    void Text::OnStringChange( const std::wstring& newString )
    {
        Update();
    }

    void Text::OnFontChange( const sf::Font& newFont )
    {
        Update();
    }

    void Text::OnCharacterSizeChange( unsigned int newCharacterSize )
    {
        Update();
    }

    void Text::Update()
    {
        auto size = Size.Get();
        const auto& game = Game::GenericGame::GetInstance();
        auto transformation = game->GetTransformation();
        m_text.setPosition( transformation.transformPoint( Position.Get() ) );
        m_text.setCharacterSize( CharacterSize.Get() );
        m_text.setFont( Font.Get() );
        m_text.setString( String.Get() );
    }
}
