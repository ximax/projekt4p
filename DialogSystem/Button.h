#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
private:
    sf::Sprite sprite;
    sf::Text text;
public:
    Button(sf::Text new_text,sf::Texture &txt, sf::Vector2f postion);
    void draw(sf::RenderWindow &window);
};


#endif // BUTTON_H
