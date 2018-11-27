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
            OnStringChange();
        }

        const sf::Font& GetFont() const
        {
            return m_font;
        }
        void SetFont( const sf::Font& value )
        {
            m_font = value;
            OnFontChange();
        }

        float GetLineHeight() const
        {
            return m_lineHeight;
        }
        void SetLineHeight( float value )
        {
            m_lineHeight = value;
            OnLineHeightChange();
        }

        void SetColor( const sf::Color value )
        {
            m_color = value;
            OnColorChange();
        }
        const sf::Color& GetColor()
        {
            return m_color;
        }

    private:
        void OnPositionChange() override;
        void OnSizeChange() override;
        void OnStringChange();
        void OnFontChange();
        void OnLineHeightChange();
        void OnColorChange();

        void UpdateCharacterSize();
        void UpdateSize();

        std::wstring m_string;
        sf::Font m_font;
        float m_lineHeight = 0.0f;
        sf::Text m_text;
        sf::Color m_color;

        float m_fontLineSpacing = 0;
    };
}