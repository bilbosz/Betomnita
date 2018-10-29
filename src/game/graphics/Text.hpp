#pragma once
#include "game/graphics/Primitive.hpp"

namespace Graphics
{
    class Text final : public Primitive
    {
    public:
        Text();
        ~Text();

        void Init();

        void Render( sf::RenderTarget& target ) override;

        const std::wstring& GetString() const
        {
            return m_string;
        }
        void SetString( const std::wstring& value )
        {
            m_string = value;
			OnStringChange( m_string );
        }

        const sf::Font& GetFont() const
        {
            return m_font;
        }
        void SetFont( const sf::Font& value )
        {
            m_font = value;
			OnFontChange( m_font );
        }

		float GetLineHeight() const
		{
			return m_lineHeight;
		}
        void SetLineHeight( float value )
        {
            m_lineHeight = value;
			OnLineHeightChange( m_lineHeight );
        }

        Utils::Property< std::wstring > String;
        Utils::Property< sf::Font > Font;
        Utils::Property< float > LineHeight;
    private:
        void OnPositionChange( const sf::Vector2f& newPosition ) override;
        void OnSizeChange( const sf::Vector2f& newSize ) override;
        void OnStringChange( const std::wstring& newString );
        void OnFontChange( const sf::Font& newFont );
        void OnLineHeightChange( float newLineHeight );

        void UpdateCharacterSize();
        void UpdateSize();

        std::wstring m_string;
        sf::Font m_font;
        float m_lineHeight = 0.0f;
        sf::Text m_text;

        float m_fontLineSpacing = 0;
    };
}