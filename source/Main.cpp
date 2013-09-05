#include <iostream> 
#include <memory>
#include <cstdlib>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "../include/Bars.hpp"

int main(int argc, char** argv) {
    unsigned width(800), height(450), numOfBars(100);
    std::unique_ptr<sf::RenderWindow> window(new sf::RenderWindow(sf::VideoMode(width, height), "Bars", sf::Style::Close));

    if (argc >= 2) {
        std::string num(argv[1]);
        if (num.at(0) == '-')
            num.assign(num, 1, num.size());
        try {
            numOfBars = std::stoul(num.c_str());
        } catch (const std::invalid_argument& arg) {
            std::cerr << "Invalid argument\n";
            return 1;
        } catch (const std::out_of_range& range) {
            std::cerr << "Out of range exception\n";
            return 2;
        }
    }   
    Bars bars(window.get(), numOfBars, width, height);
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
