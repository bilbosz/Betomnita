#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <tuple>
#include <vector>

namespace Game::Graphics
{
    class Text;
}

namespace Betomnita
{
    class MainMenuState;

    namespace Layout
    {
        class MainMenuLayout
        {
        private:
            enum class EntryId
            {
                NewGame,
                Exit,
            };
            struct Option;
            using OptionList = std::vector< Option >;

        public:
            MainMenuLayout( MainMenuState* state );
            ~MainMenuLayout();

            void Show();
            void Hide();
            void OnRender( sf::RenderTarget& target );
            void OnUpdate( const sf::Time& dt );

        private:
            struct Option
            {
                EntryId Entry;
                std::wstring Text;
                std::unique_ptr< Game::Graphics::Text > Control;
                std::function< void() > OnEnter;
            };
            MainMenuState* m_state;
            std::unique_ptr< Game::Graphics::Text > m_title;
            OptionList m_options;
        };
    }
}