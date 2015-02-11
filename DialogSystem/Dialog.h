#ifndef DIALOG_H
#define DIALOG_H
#include <SFML/Graphics.hpp>

class Dialog
{
    public:
        Dialog(sf::Color bgcolor);

        void show(sf::RenderWindow &window, sf::Text &text, sf::Vector2f _size, sf::Vector2f position); //wywietla dialog na podanym oknie, z podanym tekstem, pozycjš i rozmiarem
    private:
        sf::Color color;
};

#endif // DIALOG_H
