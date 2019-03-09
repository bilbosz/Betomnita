#pragma once
#include "app/Debug.hpp"
#include "game/event/EventInfo.hpp"

#include <vector>

namespace Game::EventSystem
{
    template< auto id >
    class Event
    {
    public:
        using Id = decltype( id );
        using Data = typename EventInfo< id >::Data;
        using Listener = typename EventInfo< id >::Listener;
        using ListenerId = typename EventInfo< id >::ListenerId;

        Event() = delete;
        Event( const Event& ) = delete;
        Event( Event&& ) = delete;
        Event& operator=( const Event& ) = delete;
        Event& operator=( Event&& ) = delete;
        ~Event() = delete;

        static auto GetId()
        {
            return id;
        }

        template< class... DataUnpacked, class = std::enable_if_t< std::is_same< Pack< DataUnpacked... >, Data >::value > >
        static void Dispatch( DataUnpacked... data )
        {
            m_executing = true;
            for( auto& listener : m_listeners )
            {
                CHECK( listener.Id != ListenerId::Undefined );
                if( listener.IsEnabled )
                {
                    CHECK( listener.Callback != nullptr );
                    listener.Callback( data... );
                }
            }
            m_executing = false;
            UpdateListeners();
        }

        static void AddListener( const Listener& listener )
        {
            if( m_executing )
            {
                m_toAddListeners.push_back( listener );
            }
            else
            {
#ifdef DEBUG
                auto it = std::find_if( m_listeners.cbegin(), m_listeners.cend(), [&listener]( const auto& item ) { return item.Id == listener.Id; } );
                CHECK( it == m_listeners.cend() );
#endif
                m_listeners.push_back( listener );
            }
        }

        static void RemoveListener( ListenerId listenerId )
        {
            if( m_executing )
            {
                m_toRemoveListeners.push_back( listenerId );
            }
            else
            {
                auto it = std::find_if( m_listeners.cbegin(), m_listeners.cend(), [listenerId]( const auto& item ) { return item.Id == listenerId; } );
                CHECK( it != m_listeners.cend() );
                m_listeners.erase( it );
            }
        }

        static Listener& GetListener( ListenerId listenerId )
        {
            auto it = std::find_if( m_listeners.begin(), m_listeners.end(), [listenerId]( const auto& item ) { return item.Id == listenerId; } );
            CHECK( it != m_listeners.end() );
            return *it;
        }

        static void UpdateListeners()
        {
            CHECK( !m_executing );
            for( const auto& listener : m_toAddListeners )
            {
                AddListener( listener );
            }
            m_toAddListeners.clear();
            for( auto listenerId : m_toRemoveListeners )
            {
                RemoveListener( listenerId );
            }
            m_toRemoveListeners.clear();
        }

    private:
        static std::vector< Listener > m_listeners;
        static std::vector< Listener > m_toAddListeners;
        static std::vector< ListenerId > m_toRemoveListeners;
        static bool m_executing;
    };

    template< auto id >
    std::vector< typename Event< id >::Listener > Event< id >::m_listeners;

    template< auto id >
    std::vector< typename Event< id >::Listener > Event< id >::m_toAddListeners;

    template< auto id >
    std::vector< typename Event< id >::ListenerId > Event< id >::m_toRemoveListeners;

    template< auto id >
    bool Event< id >::m_executing = false;
}