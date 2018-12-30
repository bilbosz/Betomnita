#include <SFML/Graphics.hpp>
#include <memory>

namespace Game::Graphics
{
    class Polygon;
}

namespace Betomnita::GamePlay
{
    class PhysicalBody
    {
    public:
        PhysicalBody();
        ~PhysicalBody();

    private:
        std::unique_ptr< Game::Graphics::Polygon > m_graphics;
        //std::unique_ptr< std::vector< sf::Vector2f > > m_physicalShape;
    };
}