#pragma once
#include "app/Debug.hpp"

#include <functional>
#include <vector>

namespace Utils
{
    template< class Type >
    class Property
    {
    public:
        using ValueType = Type;
        using ReferenceType = Type&;
        using ConstRefType = const Type&;
        using Callback = std::function< void( ConstRefType ) >;
        using Constrain = std::pair< std::function< bool( ConstRefType ) >, std::wstring >;

        Property()
        {
        }

        Property( ConstRefType value ) : m_value( value ), m_initialized( true )
        {
        }

        ~Property()
        {
        }

        template< class ReturnType = ConstRefType >
        ReturnType Get() const
        {
            ASSERT( m_initialized, "Reading uninitialized property" );
            return m_value;
        }
        void Set( ConstRefType value )
        {
            ASSERT( !m_locked, "Cannot change locked property" );
#ifdef DEBUG
            for( auto constrain : m_constrains )
            {
                ASSERT( constrain.first( value ), constrain.second );
            }
#endif
            m_value = value;
            m_initialized = true;
            std::for_each( m_callbacks.begin(), m_callbacks.end(), [this]( auto callback ) { callback( m_value ); } );
        }

        void Lock()
        {
            m_locked = true;
        }
        void Unlock()
        {
            m_locked = false;
        }

        bool IsInitialized() const
        {
            return m_initialized;
        }

        void AddCallback( Callback callback )
        {
            m_callbacks.push_back( callback );
        }

        void AddConstrain( Constrain constrain )
        {
#ifdef DEBUG
            m_constrains.push_back( constrain );
#endif
        }

    private:
        bool m_initialized = false;
        bool m_locked = false;
        ValueType m_value;
        std::vector< Callback > m_callbacks;
#ifdef DEBUG
        std::vector< Constrain > m_constrains;
#endif
    };
}