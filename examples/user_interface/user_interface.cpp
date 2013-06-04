#include <sfml/system.hpp>
#include <sfml/window.hpp>
#include <sfml/graphics.hpp>
#include <sfml/gui.hpp>

class MyButton : public sf::Button
{
public:

    MyButton() :
    sf::Button (),
    m_image(),
    m_sprite()
    {
        m_image.loadFromFile("resources/sfml_logo.png");
        m_sprite.setTexture(m_image, true);
        m_sprite.setPosition(4, 4);
    }

protected:

    void onPaint(sf::RenderTarget& target, const sf::RenderStates& states)
    {
        sf::Button::onPaint(target, states);
        target.draw(m_sprite, states);
    }

private :

    sf::Texture m_image;
    sf::Sprite  m_sprite;
};

int main()
{
    sf::GuiWindow window;

    MyButton button;
    window.addWidget(button);

    button.setSize(55, 55);

    gtk_widget_show(window.getWidgetHandler());
    gtk_widget_show(button.getWidgetHandler());

    window.setSize(640, 480);
    window.main();

    return EXIT_SUCCESS;
}
