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
        using ConstRefType = const Type&;
        using Callback = std::function< void( ConstRefType ) >;

        Property()
        {
        }

        Property( const ValueType& value ) : m_value( value ), m_initialized( true )
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
        void Set( const ValueType& value )
        {
            ASSERT( !m_locked, "Cannot change locked property" );
            m_value = value;
			m_initialized = true;
            for( auto callback : m_callbacks )
            {
                callback( m_value );
            }
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

    private:
        bool m_initialized = false;
        bool m_locked = false;
        ValueType m_value;
        std::vector< Callback > m_callbacks;
    };
}