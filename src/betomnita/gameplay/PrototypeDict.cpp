#include "betomnita/gameplay/PrototypeDict.hpp"

#include "app/Debug.hpp"
#include "betomnita/gameplay/ProjectilePrototype.hpp"
#include "betomnita/gameplay/VehicleChassisPrototype.hpp"
#include "betomnita/gameplay/VehicleGunPrototype.hpp"

#include <array>
#include <filesystem>

namespace Betomnita::GamePlay
{
    void PrototypeDict::LoadFromDirectory( const std::string& dirPath )
    {
        std::unordered_map< std::string, Prototype::Type > directoryTypeMap;
        directoryTypeMap[ "vehicles/chassis" ] = Prototype::Type::Chassis;
        directoryTypeMap[ "vehicles/guns" ] = Prototype::Type::Gun;
        directoryTypeMap[ "vehicles/projectiles" ] = Prototype::Type::Projectile;

        std::filesystem::path root( dirPath );

        for( auto& typeDir : directoryTypeMap )
        {
            std::filesystem::path currentDir( root / typeDir.first );
            for( auto& currentFile : std::filesystem::directory_iterator( currentDir ) )
            {
                CHECK( currentFile.is_regular_file() );
                std::string prototypePath = currentFile.path().lexically_normal().generic_string();
                std::unique_ptr< Prototype > prototype;
                switch( typeDir.second )
                {
                    case Prototype::Type::Chassis:
                        prototype = std::make_unique< VehicleChassisPrototype >();
                        break;
                    case Prototype::Type::Gun:
                        prototype = std::make_unique< VehicleGunPrototype >();
                        break;
                    case Prototype::Type::Projectile:
                        prototype = std::make_unique< ProjectilePrototype >();
                        break;
                    default:
                        CHECK( false );
                }
                prototype->LoadFromFile( prototypePath );
                m_prototypes[ std::move( prototypePath ) ] = std::move( prototype );
            }
        }
    }

    const Prototype& PrototypeDict::GetPrototypeByName( const std::string& name ) const
    {
        auto it = m_prototypes.find( name );
        ASSERT( it != m_prototypes.cend(), L"Prototype " << name.c_str() << L" not found" );
        return *it->second;
    }
}