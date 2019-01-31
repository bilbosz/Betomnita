#pragma once

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
            Size,
        };

        Create()

        Type GetType() const
        {
            return m_type;
        }

    private:
        Type m_type = Type::Invalid;
    };
}