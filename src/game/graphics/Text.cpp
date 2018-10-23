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

        String.Set( L"" );
        Font.Set( *Resource::DefaultFont );
        LineHeight.Set( 0.0f );

		Size.Lock();

        String.AddCallback( [this]( const std::wstring& newString ) { OnStringChange( newString ); } );
        Font.AddCallback( [this]( const sf::Font& newFont ) { OnFontChange( newFont ); } );
        LineHeight.AddCallback( [this]( float newLineHeight ) { OnLineHeightChange( newLineHeight ); } );
    }

    void Text::Render( sf::RenderTarget& target )
    {
        VERIFY( LineHeight.Get() >= 0.0f );
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
        UpdateSize();
    }

    void Text::OnFontChange( const sf::Font& newFont )
    {
        m_text.setFont( newFont );
        UpdateCharacterSize();
        UpdateSize();
    }

    void Text::OnLineHeightChange( float newLineHeight )
    {
        UpdateCharacterSize();
        UpdateSize();
    }

    void Text::UpdateCharacterSize()
    {
        auto toModelScale = Game::GenericGame::GetInstance()->GetToModelScale().x;
        const auto& font = Font.Get();

        auto initialFontLineSpacing = font.getLineSpacing( Resource::DefaultCharacterSize ) * toModelScale;
        auto initialRatio = LineHeight.Get() / initialFontLineSpacing;

        auto characterSize = static_cast< unsigned int >( Resource::DefaultCharacterSize * initialRatio );

        m_fontLineSpacing = font.getLineSpacing( characterSize ) * toModelScale;
        m_text.setCharacterSize( characterSize );
    }

    void Text::UpdateSize()
    {
		const auto game = Game::GenericGame::GetInstance();
        auto toModelScale = game->GetToModelScale().x;
		auto toModelTransform = game->GetToModelTransform();
		auto left = toModelTransform.transformPoint( { m_text.getGlobalBounds().left, 0 } );
		Size.Unlock();
        Size.Set( { left.x - Position.Get().x + m_text.getGlobalBounds().width * toModelScale, LineHeight.Get() } );
		Size.Lock();
    }
}
