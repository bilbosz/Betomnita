#include "game/graphics/View.hpp"

#include "game/GameConsts.hpp"

namespace Game::Graphics
{
    sf::Transform View::GetTransform() const
    {
        return sf::Transform().translate( 0.5f, 0.5f ).scale( Scale, Scale ).rotate( -Rotation * 180.0f / Game::Consts::Pi ).translate( -Center );
    }
}
