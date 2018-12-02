#pragma once
#include "game/event/Listener.hpp"
#include "resource/Resource.hpp"

#include <tuple>

namespace Game::EventSystem
{
    template< class EventId, EventId id >
    struct EventInfo;

    template< class ListenerId_, class... EventData >
    struct EventInfoHelper
    {
        using Data = std::tuple< EventData... >;
        using Listener = typename Listener< ListenerId_, EventData... >;
        using ListenerId = typename Listener::ListenerId;
    };
}