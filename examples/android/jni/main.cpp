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

    sf::Music music;
    if(!music.openFromFile("orchestral.ogg"))
        return EXIT_FAILURE;

    music.play();
    
    // Set the world's center to the center of the screen
    sf::View view = window.getView();
    view.setCenter(0, 0);
    window.setView(view);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                // Move the sprite to the finger's location
                sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                sprite.setPosition(worldPos);
            }
            else if (event.type == sf::Event::Resized)
            {
                // Resize the view so it's not stretched
                sf::View view = window.getView();
                view.setSize((float)event.size.width, (float)event.size.height);
                window.setView(view);
            }
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }
}
