#pragma once
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

        Property( const ValueType& value ) : m_value( value )
        {
        }

        ~Property()
        {
        }

        template< class ReturnType = ConstRefType >
        ReturnType Get() const
        {
            return m_value;
        }
        void Set( const ValueType& value )
        {
            m_value = value;
            for( auto callback : m_callbacks )
            {
                callback( m_value );
            }
        }

        void AddCallback( Callback callback )
        {
            m_callbacks.push_back( callback );
        }

    private:
        ValueType m_value;
        std::vector< Callback > m_callbacks;
    };
}