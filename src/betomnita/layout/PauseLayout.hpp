#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <tuple>
#include <vector>

namespace Game::Graphics
{
    class Polygon;
    class Text;
}

namespace Betomnita
{
    class PauseState;

    namespace Layout
    {
        class PauseLayout
        {
        private:
            enum class EntryId
            {
                Resume,
                MainMenu,
                ExitGame,
            };
            struct Option;
            using OptionList = std::vector< Option >;

        public:
            PauseLayout( PauseState* state );
            ~PauseLayout();

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
            PauseState* m_state;
            std::unique_ptr< Game::Graphics::Polygon > m_shroud;
            std::unique_ptr< Game::Graphics::Text > m_title;
            OptionList m_options;
        };
    }
}