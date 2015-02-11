#include "Button.h"

Button::Button(sf::Text new_text,sf::Texture &txt, sf::Vector2f postion)
{
    sprite.setTexture(txt);

    text = new_text;
    sprite.setPosition(postion);
    text.setPosition(postion.x + 1, postion.y + 1);
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(text); //tutaj siê sypie
}
