#pragma once
#include "betomnita/gameplay/Prototype.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace Betomnita::GamePlay
{
    class Prototype;

    class PrototypeDict final
    {
    public:
        void LoadFromDirectory( const std::string& dirPath );
        const Prototype& GetPrototypeByName( const std::string& name ) const;

    private:
        std::unordered_map< std::string, std::unique_ptr< Prototype > > m_prototypes;
    };
}