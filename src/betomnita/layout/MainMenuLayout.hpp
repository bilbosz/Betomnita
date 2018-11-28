#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <tuple>
#include <vector>

namespace Graphics
{
    class Text;
}

namespace Betomnita::Layout
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

        MainMenuLayout();
        ~MainMenuLayout();

        void Show();
        void Hide();
        void OnRender( sf::RenderTarget& target );

    private:
        struct Option
        {
            EntryId Entry;
            std::wstring Text;
            std::unique_ptr< Graphics::Text > Control;
            std::function< void() > OnEnter;
        };
        std::unique_ptr< Graphics::Text > m_title;
        OptionList m_options;
    };
}