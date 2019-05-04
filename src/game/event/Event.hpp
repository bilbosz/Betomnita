#pragma once
#include "app/Debug.hpp"
#include "game/event/EventInfo.hpp"

#include <queue>
#include <variant>
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

        enum class Command
        {
            AddListener = 0,
            RemoveListener,
            EnableListener,
            DisableListener,
            Size,
        };

        struct CommandData
        {
            Event< id >::Command Command;
            std::variant< Listener, ListenerId > Data;
        };

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

        static void AddListener( const Listener& listener, bool force = false )
        {
            if( force )
            {
                m_listeners.push_back( listener );
            }
            else
            {
                m_commands.push( { Command::AddListener, listener } );
                if( !m_executing )
                {
                    UpdateListeners();
                }
            }
        }

        static void RemoveListener( ListenerId listenerId, bool force = false )
        {
            if( force )
            {
                auto it = std::find_if( m_listeners.cbegin(), m_listeners.cend(), [listenerId]( const auto& item ) { return item.Id == listenerId; } );
                CHECK( it != m_listeners.cend() );
                m_listeners.erase( it );
            }
            else
            {
                m_commands.push( { Command::RemoveListener, listenerId } );
                if( !m_executing )
                {
                    UpdateListeners();
                }
            }
        }

        static void EnableListener( ListenerId listenerId, bool force = false )
        {
            if( force )
            {
                auto it = std::find_if( m_listeners.begin(), m_listeners.end(), [listenerId]( const auto& item ) { return item.Id == listenerId; } );
                CHECK( it != m_listeners.end() );
                it->IsEnabled = true;
            }
            else
            {
                m_commands.push( { Command::EnableListener, listenerId } );
                if( !m_executing )
                {
                    UpdateListeners();
                }
            }
        }

        static void DisableListener( ListenerId listenerId, bool force = false )
        {
            if( force )
            {
                auto it = std::find_if( m_listeners.begin(), m_listeners.end(), [listenerId]( const auto& item ) { return item.Id == listenerId; } );
                CHECK( it != m_listeners.end() );
                it->IsEnabled = false;
            }
            else
            {
                m_commands.push( { Command::DisableListener, listenerId } );
                if( !m_executing )
                {
                    UpdateListeners();
                }
            }
        }

        static void UpdateListeners()
        {
            CHECK( !m_executing );
            if( m_commands.empty() )
            {
                return;
            }
            const auto& commandData = m_commands.front();
            switch( commandData.Command )
            {
                case Command::AddListener:
                {
                    AddListener( std::get< Listener >( commandData.Data ), true );
                }
                break;
                case Command::RemoveListener:
                {
                    RemoveListener( std::get< ListenerId >( commandData.Data ), true );
                }
                break;
                case Command::EnableListener:
                {
                    EnableListener( std::get< ListenerId >( commandData.Data ), true );
                }
                break;
                case Command::DisableListener:
                {
                    DisableListener( std::get< ListenerId >( commandData.Data ), true );
                }
                break;
            }
            m_commands.pop();
            UpdateListeners();
        }

    private:
        static std::vector< Listener > m_listeners;
        static std::queue< CommandData > m_commands;
        static bool m_executing;
    };

    template< auto id >
    std::vector< typename Event< id >::Listener > Event< id >::m_listeners;

    template< auto id >
    std::queue< typename Event< id >::CommandData > Event< id >::m_commands;

    template< auto id >
    bool Event< id >::m_executing = false;
}