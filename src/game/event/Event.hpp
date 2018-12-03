#pragma once
#include "game/event/EventInfo.hpp"

#include <vector>

namespace Game::EventSystem
{
    template< class Id, Id id >
    class Event
    {
    public:
        using Data = typename EventInfo< Id, id >::Data;
        using Listener = typename EventInfo< Id, id >::Listener;
        using ListenerId = typename EventInfo< Id, id >::ListenerId;

        Event() = delete;

        static Id GetId()
        {
            return id;
        }

        template< class... DataUnpacked >
        static void Dispatch( DataUnpacked... data )
        {
            static_assert( std::is_same< Pack< DataUnpacked... >, Data >::value, "Event and listener data does not match in function " __FUNCTION__ );
            for( auto& listener : m_listeners )
            {
                CHECK( listener.Id != ListenerId::Undefined );
                if( listener.IsEnabled )
                {
                    CHECK( listener.Callback != nullptr );
                    listener.Callback( data... );
                }
            }
        }

        static void AddListener( const Listener& listener )
        {
#ifdef DEBUG
            auto it = std::find_if( m_listeners.cbegin(), m_listeners.cend(), [&listener]( const auto& item ) { return item.Id == listener.Id; } );
            CHECK( it == m_listeners.cend() );
#endif
            m_listeners.push_back( listener );
        }

        static void AddListenerAfter( const Listener& listener, ListenerId afterListener )
        {
            auto it = std::find_if( m_listeners.cbegin(), m_listeners.cend(), [afterListener]( const auto& item ) { return item.Id == afterListener; } );
            CHECK( it == m_listeners.cend() );
            m_listeners.insert( it + 1, listener );
        }

        static void AddListenerBefore( const Listener& listener, ListenerId beforeListener )
        {
            auto it = std::find_if( m_listeners.cbegin(), m_listeners.cend(), [beforeListener]( const auto& item ) { return item.Id == beforeListener; } );
            CHECK( it == m_listeners.cend() );
            m_listeners.insert( it, listener );
        }

        static void RemoveListener( ListenerId listenerId )
        {
            auto it = std::find_if( m_listeners.cbegin(), m_listeners.cend(), [listenerId]( const auto& item ) { return item.Id == listenerId; } );
            CHECK( it != m_listeners.cend() );
            m_listeners.erase( it );
        }

        static Listener& GetListener( ListenerId listenerId )
        {
            auto it = std::find_if( m_listeners.begin(), m_listeners.end(), [listenerId]( const auto& item ) { return item.Id == listenerId; } );
            CHECK( it != m_listeners.end() );
            return *it;
        }

    private:
        static std::vector< Listener > m_listeners;
    };

    template< class Id, Id id >
    std::vector< typename Event< Id, id >::Listener > Event< Id, id >::m_listeners;
}