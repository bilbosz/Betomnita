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

        Utils::Property< std::wstring > String;
        Utils::Property< sf::Font > Font;
		Utils::Property< unsigned int > CharacterSize;

    private:
        void OnPositionChange( const sf::Vector2f& newPosition ) override;
        void OnSizeChange( const sf::Vector2f& newSize ) override;
        void OnStringChange( const std::wstring& newString );
        void OnFontChange( const sf::Font& newFont );
		void OnCharacterSizeChange( unsigned int newCharacterSize );

        void PrepareRender();

		sf::Text m_text;
    };
}