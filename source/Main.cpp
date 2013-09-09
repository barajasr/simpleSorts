#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "../include/Bars.hpp"

int main(void) {
    unsigned width(800), height(450), numOfBars(100);
    std::unique_ptr<sf::RenderWindow> window(new sf::RenderWindow(
        sf::VideoMode(width, height), "Bars", sf::Style::Close));

    Bars bars(window.get(), numOfBars, width, height);
    if (!bars.isValid())
        return 1;

    sf::Event event;
    while (window->isOpen()) {
        while (window->pollEvent(event)){ 
            if (event.type == sf::Event::Closed)
            window->close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::B)
                    bars.bubbleSort();
                else if (event.key.code == sf::Keyboard::I)
                    bars.insertionSort();
                else if (event.key.code == sf::Keyboard::M)
                    bars.mergeSort();
                else if (event.key.code == sf::Keyboard::R)
                    bars.shuffleBars();
                else if (event.key.code == sf::Keyboard::S)
                    bars.selectionSort();
            }
        }
        bars.render();
    }
    return 0;
}
