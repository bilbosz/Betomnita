#pragma once
#include <string>

namespace Betomnita::GamePlay
{
    class Prototype
    {
    public:
        enum class Type
        {
            Invalid = -1,
            Chassis,
            Gun,
            Projectile,
            Size,
        };

        Prototype( Type type ) : m_type( type )
        {
        }

        virtual ~Prototype()
        {
        }

        Type GetType() const
        {
            return m_type;
        }

        virtual void LoadFromFile( const std::string& filename ) = 0;

    private:
        Type m_type = Type::Invalid;
    };
}