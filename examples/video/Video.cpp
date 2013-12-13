#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Video.hpp>
#include <iostream>


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML - Video");

    sf::VideoBuffer videobuffer;
    videobuffer.loadFromFile("resources/sample.ogv");

    std::cout << "Remove this print statement and you'll get a segmentation fault (O.o)" << std::endl;

    sf::Video video(videobuffer);
    video.play();

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
        window.draw(video);
        window.display();
    }

    return EXIT_SUCCESS;
}
