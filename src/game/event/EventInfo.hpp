#pragma once
#include "game/event/Listener.hpp"
#include "resource/Resource.hpp"

#include <tuple>

namespace Game::EventSystem
{
    template< auto id >
    struct EventInfo;

    template< class... Args >
    struct Pack
    {
    };

    template< class ListenerId_, class... EventData >
    struct EventInfoHelper
    {
        using Data = Pack< EventData... >;
        using Listener = typename Listener< ListenerId_, EventData... >;
        using ListenerId = typename Listener::ListenerId;
    };
}