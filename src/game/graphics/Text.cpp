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
        m_text.setCharacterSize( Resource::DefaultCharacterSize );
        String.AddCallback( [this]( const std::wstring& newString ) { OnStringChange( newString ); } );
        Font.AddCallback( [this]( const sf::Font& newFont ) { OnFontChange( newFont ); } );
        LineHeight.AddCallback( [this]( float newLineHeight ) { OnLineHeightChange( newLineHeight ); } );
    }

    void Text::Render( sf::RenderTarget& target )
    {
        Primitive::Render( target );
        target.draw( m_text );
    }

    void Text::OnPositionChange( const sf::Vector2f& newPosition )
    {
        m_text.setPosition( Game::GenericGame::GetInstance()->GetToScreenTransform().transformPoint( newPosition ) );
        Primitive::OnPositionChange( newPosition );
    }

    void Text::OnSizeChange( const sf::Vector2f& newSize )
    {
        Primitive::OnSizeChange( newSize );
    }

    void Text::OnStringChange( const std::wstring& newString )
    {
        m_text.setString( newString );
    }

    void Text::OnFontChange( const sf::Font& newFont )
    {
        m_text.setFont( newFont );
    }

    void Text::OnLineHeightChange( float newLineHeight )
    {
        const auto game = Game::GenericGame::GetInstance();
        auto toModelScale = game->GetToModelScale().x;
        const auto& font = Font.Get();

        auto initialFontLineSpacing = font.getLineSpacing( Resource::DefaultCharacterSize ) * toModelScale;
        auto initialRatio = newLineHeight / initialFontLineSpacing;

        auto characterSize = static_cast< unsigned int >( Resource::DefaultCharacterSize * initialRatio );
        auto fontLineSpacing = font.getLineSpacing( characterSize ) * toModelScale;

        m_text.setCharacterSize( characterSize );

        Size.Set( { m_text.getGlobalBounds().width * toModelScale, fontLineSpacing } );
    }
}
