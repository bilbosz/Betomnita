#pragma once
#include "game/State.hpp"

#include "app/Debug.hpp"

#include <map>
#include <memory>
#include <vector>

namespace Game
{
    template< class StateId >
    class StateMachine final
    {
    public:
        using StateType = State< StateId >;
        using StatePtr = std::shared_ptr< StateType >;
        using StateWeakPtr = std::weak_ptr< StateType >;

        void RegisterState( StatePtr state );
        void UnregisterState( StateId id );
        bool IsStateRegistered( StateId id ) const
        {
            VERIFY( static_cast< size_t >( id ) < static_cast< size_t >( StateId::Size ) );
            return m_registeredStates[ static_cast< size_t >( id ) ] != nullptr;
        }

        void PushState( StateId id );
        void PopState( StateId id );
        bool IsStateActive( StateId id ) const;
        StateWeakPtr GetForeground() const;
        bool IsStackEmpty() const
        {
            return m_activeStates.empty();
        }

        void OnUpdate( const sf::Time& dt );

    private:
        std::array< StatePtr, static_cast< size_t >( StateId::Size ) > m_registeredStates;
        std::vector< StatePtr > m_activeStates;
    };

    template< class StateId >
    void StateMachine< StateId >::RegisterState( StatePtr state )
    {
        VERIFY( !IsStateRegistered( state->GetId() ) );
        
        m_registeredStates[ static_cast< size_t >( state->GetId() ) ] = state;
        state->m_isRegistered = true;
        state->OnRegister();
    }

    template< class StateId >
    void StateMachine< StateId >::UnregisterState( StateId id )
    {
        VERIFY( IsStateRegistered( id ) );
        VERIFY( !IsStateActive( id ) )
        
        auto& toUnregister = m_registeredStates[ static_cast< size_t >( id ) ];
        toUnregister->m_isRegistered = false;
        toUnregister->OnUnregister();
        toUnregister.reset();
    }

    template< class StateId >
    void StateMachine< StateId >::PushState( StateId id )
    {
        VERIFY( IsStateRegistered( id ) );
        VERIFY( !IsStateActive( id ) );
        
        if( !IsStackEmpty() )
        {
            auto& fg = m_activeStates.back();
            fg->m_isForeground = false;
            fg->OnBackground();
        }

        m_activeStates.emplace_back( m_registeredStates[ static_cast< size_t >( id ) ] );
        auto& fg = m_activeStates.back();
        fg->m_isActive = true;
        fg->OnActivate();
        fg->m_isForeground = true;
        fg->OnForeground();
    }

    template< class StateId >
    void StateMachine< StateId >::PopState( StateId id )
    {
        VERIFY( !IsStackEmpty() );
        VERIFY( m_activeStates.back()->GetId() == id );

        auto& fg = m_activeStates.back();
        fg->m_isForeground = false;
        fg->OnBackground();
        fg->m_isActive = false;
        fg->OnDeactivate();
        m_activeStates.pop_back();
        
        if( !IsStackEmpty() )
        {
            auto& fg = m_activeStates.back();
            fg->m_isForeground = true;
            fg->OnForeground();
        }
    }

    template< class StateId >
    bool StateMachine< StateId >::IsStateActive( StateId id ) const
    {
        VERIFY( IsStateRegistered( id ) );
        return m_registeredStates[ static_cast< size_t >( id ) ]->IsActive();
    }

    template< class StateId >
    typename StateMachine< StateId >::StateWeakPtr StateMachine< StateId >::GetForeground() const
    {
        VERIFY( !IsStackEmpty() );
        return m_activeStates.back();
    }

    template< class StateId >
    void StateMachine< StateId >::OnUpdate( const sf::Time& dt )
    {
        for( auto& state : m_activeStates )
        {
            state->OnUpdate( dt );
        }
    }
}