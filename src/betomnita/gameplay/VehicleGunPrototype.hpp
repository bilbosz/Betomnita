#pragma once
#include "betomnita/gameplay/Prototype.hpp"

namespace Betomnita::GamePlay
{
    class VehicleGunPrototype : public Prototype
    {
    public:
        VehicleGunPrototype();

        void LoadFromFile( const std::string& filename ) override;

    private:
    };
}