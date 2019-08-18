#pragma once
#include "app/Debug.hpp"

#include <SFML/Graphics.hpp>

namespace Game
{
    template< class Id >
    class State
    {
    public:
        template< class Id_ >
        friend class StateMachine;

        explicit State( Id id ) : m_id( id )
        {
        }
        virtual ~State() = default;

        Id GetId() const
        {
            return m_id;
        }

        [[nodiscard]] bool IsRegistered() const
        {
            return m_isRegistered;
        }

        [[nodiscard]] bool IsActive() const
        {
            return m_isActive;
        }

        [[nodiscard]] bool IsForeground() const
        {
            return m_isForeground;
        }

    protected:
        virtual void OnRegister()
        {
#ifdef SHOW_STATE_LOG
            MESSAGE( L"STATE registered: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnUnregister()
        {
#ifdef SHOW_STATE_LOG
            MESSAGE( L"STATE unregistered: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnActivate()
        {
#ifdef SHOW_STATE_LOG
            MESSAGE( L"STATE activated: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnDeactivate()
        {
#ifdef SHOW_STATE_LOG
            MESSAGE( L"STATE deactivated: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnForeground()
        {
#ifdef SHOW_STATE_LOG
            MESSAGE( L"STATE foregrounded: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnBackground()
        {
#ifdef SHOW_STATE_LOG
            MESSAGE( L"STATE state backgrounded: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnUpdate( const sf::Time& dt )
        {
        }

        virtual void OnRender( sf::RenderTarget& target )
        {
        }

    private:
        Id m_id;
        bool m_isRegistered = false;
        bool m_isActive = false;
        bool m_isForeground = false;
    };
}