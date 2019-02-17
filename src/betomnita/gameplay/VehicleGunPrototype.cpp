#include "betomnita/gameplay/VehicleGunPrototype.hpp"

#include "app/Debug.hpp"

namespace Betomnita::GamePlay
{
    VehicleGunPrototype::VehicleGunPrototype() : Prototype( Type::Gun )
    {
    }

    void VehicleGunPrototype::LoadFromFile( const std::string& filename )
    {
        MESSAGE( L"VGP: " << filename.c_str() );
    }
}