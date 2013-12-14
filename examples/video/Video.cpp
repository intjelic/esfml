#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Video.hpp>
#include <iostream>

#if defined(SFML_SYSTEM_LINUX)
    #include <X11/Xlib.h>
#endif

int main()
{
    #if defined(SFML_SYSTEM_LINUX)
        XInitThreads();
    #endif

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML - Video");

    sf::Movie movie;
    movie.openFromFile("resources/sample.ogv");
    movie.play();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                       window.close();

                else if (event.key.code == sf::Keyboard::P)
                       movie.pause();

                else if (event.key.code == sf::Keyboard::S)
                       movie.stop();

                else if (event.key.code == sf::Keyboard::R)
                       movie.play();
            }
        }

        window.clear();
        window.draw(movie);
        window.display();
    }

    return EXIT_SUCCESS;
}
