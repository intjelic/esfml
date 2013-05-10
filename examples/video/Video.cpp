#include <sfml/system.hpp>
#include <sfml/graphics.hpp>
#include <sfml/video.hpp>

int main()
{
	sf::VideoBuffer videobuffer;
	videobuffer.loadFromFile("resources/sample.ogv", 50);

    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML - Video");

	sf::Texture texture;
	texture.loadFromImage(videobuffer.getFrames()[42]);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();
       }

		window.clear();
		window.draw(sf::Sprite(texture));
        window.display();
    }

    return EXIT_SUCCESS;
}
