#include <sfml/system.hpp>
#include <sfml/window.hpp>
#include <sfml/graphics.hpp>

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");

    sf::Texture texture;
    texture.loadFromFile("image.png");

    sf::Sprite sprite(texture);

    sf::Vector2u imageSize = texture.getSize();
    sprite.setOrigin(imageSize.x/2, imageSize.y/2);

    sf::Vector2u screenSize = window.getSize();
    sprite.setPosition(screenSize.x/2, screenSize.y/2);

    while (window.isOpen())
    {
        sf::Event event;
        if (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                sprite.setPosition(event.mouseMove.x, event.mouseMove.y);
            }
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }
}
