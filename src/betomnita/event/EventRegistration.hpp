#pragma once
#include "game/event/Event.hpp"
#include "resource/Resource.hpp"

namespace Game::EventSystem
{
#define REGISTER_EVENT_WO_ARGS( NAME ) \
    template<> \
    struct EventInfo< Resource::EventId, Resource::EventId::NAME > \
    { \
    public: \
        using Hlpr = EventInfoHelper< Resource::ListenerId >; \
        using Data = Hlpr::Data; \
        using Listener = Hlpr::Listener; \
        using ListenerId = Hlpr::ListenerId; \
    }

#define REGISTER_EVENT_W_ARGS( NAME, ... ) \
    template<> \
    struct EventInfo< Resource::EventId, Resource::EventId::NAME > \
    { \
    public: \
        using Hlpr = EventInfoHelper< Resource::ListenerId, __VA_ARGS__ >; \
        using Data = Hlpr::Data; \
        using Listener = Hlpr::Listener; \
        using ListenerId = Hlpr::ListenerId; \
    }

    REGISTER_EVENT_WO_ARGS( OnStart );
    REGISTER_EVENT_W_ARGS( OnUpdate, const sf::Time& );
    REGISTER_EVENT_W_ARGS( OnRender, sf::RenderTarget& );
    REGISTER_EVENT_WO_ARGS( OnVideoSettingChanged );
    REGISTER_EVENT_WO_ARGS( OnClose );
    REGISTER_EVENT_W_ARGS( OnKeyPressed, const sf::Event::KeyEvent& );
    REGISTER_EVENT_W_ARGS( OnKeyReleased, const sf::Event::KeyEvent& );
    REGISTER_EVENT_W_ARGS( OnMouseButtonPressed, const sf::Vector2f&, sf::Mouse::Button );
    REGISTER_EVENT_W_ARGS( OnMouseButtonReleased, const sf::Vector2f&, sf::Mouse::Button );
    REGISTER_EVENT_W_ARGS( OnMouseMoved, const sf::Vector2f& );

#undef REGISTER_EVENT_WO_ARGS
#undef REGISTER_EVENT_W_ARGS
}