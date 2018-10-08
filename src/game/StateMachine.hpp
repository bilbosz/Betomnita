#pragma once
#include "game/State.hpp"

#include <map>
#include <memory>
#include <vector>

namespace Game
{
    class State;

    class StateMachine
    {
    public:
        using IdType = int;

        void RegisterState( IdType id, std::shared_ptr< State > state );
        void UnregisterState( IdType id );
        bool IsStateRegistered( IdType id ) const
        {
            return m_registredStates.find( id ) != m_registredStates.cend();
        }

        void PushState( IdType id );
        void PopState( IdType id );
        bool IsStateUsed( IdType id ) const;
        std::weak_ptr< State > TopState() const;
        bool IsStackEmpty() const
        {
            return m_stack.empty();
        }

        void OnUpdate( sf::Time dt );

    private:
        std::map< IdType, std::shared_ptr< State > > m_registredStates;
        std::vector< std::pair< IdType, std::shared_ptr< State > > > m_stack;
        std::shared_ptr< State > m_top;
    };
}