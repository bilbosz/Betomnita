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
    public:
        enum class EntryId
        {
            NewGame,
            Exit,
        };
        struct Option
        {
            EntryId Entry;
            std::wstring Text;
            std::function< void() > OnEnter;
        };
        using OptionList = std::vector< Option >;

        MainMenuLayout();
        ~MainMenuLayout();

        void Show();
        void Hide();
        void OnRender( sf::RenderTarget& target );

    private:
        std::unique_ptr< Graphics::Text > m_title;
        OptionList m_options;
    };
}