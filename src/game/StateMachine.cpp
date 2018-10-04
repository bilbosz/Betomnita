#include "game/StateMachine.hpp"

#include "app/Debug.hpp"

namespace Game
{
    void StateMachine::RegisterState( IdType id, std::shared_ptr< State > state )
    {
        VERIFY( !IsStateRegistered( id ) );
        m_registredStates[ id ] = state;
    }

    void StateMachine::UnregisterState( IdType id )
    {
        VERIFY( IsStateRegistered( id ) );
        m_registredStates.erase( id );
    }

    void StateMachine::PushState( IdType id )
    {
        VERIFY( IsStateRegistered( id ) );
        VERIFY( IsStateUsed( id ) );
        m_stack.push_back( std::make_pair( id, m_registredStates[ id ] ) );
    }

    void StateMachine::PopState( IdType id )
    {
        VERIFY( !IsStackEmpty() );
        VERIFY( m_stack.back().first == id );
        m_stack.pop_back();
    }

    bool StateMachine::IsStateUsed( IdType id ) const
    {
        VERIFY( IsStateRegistered( id ) );
        return std::find( m_stack.cbegin(), m_stack.cend(), std::make_pair( id, m_registredStates.at( id ) ) ) == m_stack.cend();
    }

    std::weak_ptr< State > StateMachine::TopState() const
    {
        VERIFY( !IsStackEmpty() );
        return m_stack.back().second;
    }

    void StateMachine::OnUpdate( sf::Time dt )
    {
        auto currentState = TopState().lock();
        if( currentState )
        {
            currentState->OnUpdate( dt );
        }
    }
}