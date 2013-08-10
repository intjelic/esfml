#include <sfml/system.hpp>
#include <sfml/graphics.hpp>
#include <sfml/video.hpp>
#include <iostream>


int main()
{
    sf::VideoBuffer videobuffer;
    videobuffer.loadFromFile("resources/sample.ogv");

    sf::Video video(videobuffer);
    video.play();

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML - Video");
    std::cout << "Remove this print statement and you'll get a segmentation fault (O.o)" << std::endl;

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
        video.update();
        window.draw(video);
        window.display();
    }

    return EXIT_SUCCESS;
}
