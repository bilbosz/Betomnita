#pragma once

namespace Game::EventSystem
{
    template< class ListenerId_, class... EventData >
    struct Listener
    {
        using ListenerId = ListenerId_;

        ListenerId Id = ListenerId::Undefined;
        bool IsEnabled = false;
        std::function< void( EventData... ) > Callback = nullptr;
    };
}