#include <sfml/system.hpp>
#include <sfml/window.hpp>
#include <sfml/graphics.hpp>
#include <sfml/audio.hpp>

#include <iostream>

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");

    sf::Texture texture;
    if(!texture.loadFromFile("image.png"))
    	return EXIT_FAILURE;

    sf::Sprite sprite(texture);

    sf::Vector2u imageSize = texture.getSize();
    sprite.setOrigin(imageSize.x/2, imageSize.y/2);

    sf::Vector2u screenSize = window.getSize();
    sprite.setPosition(screenSize.x/2, screenSize.y/2);

    sf::Music music;
    if(!music.openFromFile("orchestral.ogg"))
    	return EXIT_FAILURE;

    music.play();

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
