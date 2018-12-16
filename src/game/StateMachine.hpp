#pragma once
#include "game/State.hpp"

#include "app/Debug.hpp"

#include <SFML/Graphics.hpp>
#include <array>
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

        void RegisterState( StatePtr state );
        void UnregisterState( StateId id );
        bool IsStateRegistered( StateId id ) const
        {
            CHECK( static_cast< size_t >( id ) < static_cast< size_t >( StateId::Size ) );
            return m_registeredStates[ static_cast< size_t >( id ) ] != nullptr;
        }
        StatePtr GetState( StateId id ) const
        {
            CHECK( IsStateRegistered( id ) );
            return m_registeredStates[ static_cast< size_t >( id ) ];
        }

        void PushState( StateId id );
        void PopState( StateId id );
        bool IsStateActive( StateId id ) const;
        StatePtr GetForeground() const;
        bool IsStackEmpty() const
        {
            return m_activeStates.empty();
        }

        void OnUpdate( const sf::Time& dt );
        void OnRender( sf::RenderTarget& target );

    private:
        std::array< StatePtr, static_cast< size_t >( StateId::Size ) > m_registeredStates;
        std::vector< StatePtr > m_activeStates;
    };

    template< class StateId >
    void StateMachine< StateId >::RegisterState( StatePtr state )
    {
        CHECK( !IsStateRegistered( state->GetId() ) );

        m_registeredStates[ static_cast< size_t >( state->GetId() ) ] = state;
        state->m_isRegistered = true;
        state->OnRegister();
    }

    template< class StateId >
    void StateMachine< StateId >::UnregisterState( StateId id )
    {
        CHECK( IsStateRegistered( id ) );
        CHECK( !IsStateActive( id ) );

        auto& toUnregister = m_registeredStates[ static_cast< size_t >( id ) ];
        toUnregister->m_isRegistered = false;
        toUnregister->OnUnregister();
        toUnregister.reset();
    }

    template< class StateId >
    void StateMachine< StateId >::PushState( StateId id )
    {
        CHECK( IsStateRegistered( id ) );
        CHECK( !IsStateActive( id ) );

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
        CHECK( !IsStackEmpty() );
        CHECK( m_activeStates.back()->GetId() == id );

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
        CHECK( IsStateRegistered( id ) );
        return m_registeredStates[ static_cast< size_t >( id ) ]->IsActive();
    }

    template< class StateId >
    typename StateMachine< StateId >::StatePtr StateMachine< StateId >::GetForeground() const
    {
        CHECK( !IsStackEmpty() );
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

    template< class StateId >
    inline void StateMachine< StateId >::OnRender( sf::RenderTarget& target )
    {
        for( auto& state : m_activeStates )
        {
            state->OnRender( target );
        }
    }
}