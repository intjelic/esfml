#include <map>
#include <sfml/system.hpp>
#include <sfml/window.hpp>
#include <sfml/graphics.hpp>
#include <sfml/audio.hpp>


int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");

    sf::Texture texture;
    if(!texture.loadFromFile("image.png"))
        return EXIT_FAILURE;

    // An array of sprites, one for each finger touching the screen
    std::map<int, sf::Sprite> sprites;
    std::map<int, sf::Sprite>::iterator sprite;

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
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                int index = static_cast<int>(event.mouseButton.button);

                sprites[index] = sf::Sprite(texture);
                sf::Vector2u imageSize = texture.getSize();
                sprites[index].setOrigin(imageSize.x / 2, imageSize.y / 2);

                // Move the sprite to the finger's location
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                sprites[index].setPosition(worldPos);
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                int index = static_cast<int>(event.mouseButton.button);
                sprites.erase(index);
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                int index = static_cast<int>(event.mouseButton.button);

                // Move the sprite to the finger's location
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                sprites[index].setPosition(worldPos);
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

        for (std::map<int, sf::Sprite>::iterator it=sprites.begin(); it!=sprites.end(); ++it)
            window.draw(it->second);

        window.display();
    }
}

