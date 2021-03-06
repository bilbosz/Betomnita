#include "game/graphics/Text.hpp"

#include "app/Debug.hpp"
#include "game/GameConsts.hpp"
#include "game/GenericGame.hpp"

namespace Game::Graphics
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
    }

    void Text::Render( sf::RenderTarget& target, const sf::Transform& transform )
    {
        VERIFY( m_lineHeight >= 0.0f );
        if( m_lineHeight == 0.0f )
        {
            Primitive::Render( target, transform );
            return;
        }
        Primitive::Render( target, transform );
        target.draw( m_text, transform );
    }

    void Text::OnPositionChange()
    {
        m_text.setPosition( Game::GenericGame::GetInstance()->GetToScreenTransform().transformPoint( m_position ) );
        Primitive::OnPositionChange();
    }

    void Text::OnSizeChange()
    {
        ASSERT( false, L"Text should not be changed by changing size but by line height." );
        Primitive::OnSizeChange();
    }

    void Text::OnStringChange()
    {
        m_text.setString( m_string );
        UpdateSize();
    }

    void Text::OnFontChange()
    {
        m_text.setFont( m_font );
        UpdateCharacterSize();
        UpdateSize();
    }

    void Text::OnLineHeightChange()
    {
        ASSERT( m_lineHeight > 0.0f, L"Line height has to be greater than zero" );
        UpdateCharacterSize();
        UpdateSize();
    }

    void Text::OnColorChange()
    {
        m_text.setFillColor( m_color );
    }

    void Text::UpdateCharacterSize()
    {
        auto toModelScale = Game::GenericGame::GetInstance()->GetToModelScale().x;

        auto initialFontLineSpacing = m_font.getLineSpacing( Game::Consts::DefaultCharacterSize ) * toModelScale;
        auto initialRatio = m_lineHeight / initialFontLineSpacing;

        auto characterSize = static_cast< unsigned int >( Game::Consts::DefaultCharacterSize * initialRatio );

        m_fontLineSpacing = m_font.getLineSpacing( characterSize ) * toModelScale;
        m_text.setCharacterSize( characterSize );
    }

    void Text::UpdateSize()
    {
        const auto game = Game::GenericGame::GetInstance();
        auto toModelScale = game->GetToModelScale().x;
        m_size = { m_text.getGlobalBounds().width * toModelScale, m_lineHeight };
        Primitive::OnSizeChange();
    }
}
