#include "Dialog.h"

Dialog::Dialog(sf::Color bgcolor)
{
    color = bgcolor;
}

void Dialog::show(sf::RenderWindow &window, sf::Text &text, sf::Vector2f _size, sf::Vector2f position)
{
    sf::RectangleShape background(_size);
    background.setPosition(position);
    background.setFillColor(color);

    text.setPosition(sf::Vector2f(position.x + 20, position.y + 20)); //ustawiamy marginesy; 20px odstêpu tekstu od lewej strony i 20px od góry

    window.draw(background);
    window.draw(text);
}
