#pragma once
#include "game/event/Event.hpp"
#include "resource/Resource.hpp"

namespace Game::EventSystem
{
    template<>
    struct EventInfo< Resource::EventId, Resource::EventId::OnKeyPressed >
    {
    public:
        using Hlpr = EventInfoHelper< Resource::ListenerId, const sf::Event::KeyEvent& >;
        using Data = Hlpr::Data;
        using Listener = Hlpr::Listener;
        using ListenerId = Hlpr::ListenerId;
    };
}