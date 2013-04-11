#include <sfml/system.hpp>
#include <sfml/window.hpp>
#include <GLES/gl.h>

int main(int argc, char *argv[])
{
    sf::Window window(sf::VideoMode::getDesktopMode(), "");

    while (window.isOpen())
    {
        sf::Event event;
        if (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        glClearColor(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        window.display();
    }
}
