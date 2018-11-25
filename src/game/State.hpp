#pragma once
#include "app/Debug.hpp"

#include <SFML/System.hpp>


namespace Game
{
    template< class Id >
    class State
    {
    public:
        template< class Id >
        friend class StateMachine;

        State( Id id ) : m_id( id )
        {
        }
        virtual ~State()
        {
        }

        Id GetId() const
        {
            return m_id;
        }

        bool IsRegistered() const
        {
            return m_isRegistered;
        }

        bool IsActive() const
        {
            return m_isActive;
        }

        bool IsForeground() const
        {
            return m_isForeground;
        }

    protected:
        virtual void OnRegister()
        {
#ifdef DEBUG
            MESSAGE( L"STATE registered: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnUnregister()
        {
#ifdef DEBUG
            MESSAGE( L"STATE unregistered: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnActivate()
        {
#ifdef DEBUG
            MESSAGE( L"STATE activated: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnDeactivate()
        {
#ifdef DEBUG
            MESSAGE( L"STATE deactivated: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnForeground()
        {
#ifdef DEBUG
            MESSAGE( L"STATE foregrounded: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnBackground()
        {
#ifdef DEBUG
            MESSAGE( L"STATE state backgrounded: " << static_cast< int >( GetId() ) );
#endif
        }
        virtual void OnUpdate( const sf::Time& dt )
        {
#ifdef DEBUG
            //MESSAGE( L"STATE updated: " << static_cast< int >( GetId() ) );
#endif
        }

    private:
        Id m_id;
        bool m_isRegistered = false;
        bool m_isActive = false;
        bool m_isForeground = false;
    };
}