#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "../include/Bar.hpp"

Bar::Bar() : Sorted(false), Current(false), Value(0), Rect(nullptr) {}

Bar::Bar(const unsigned value, const unsigned width, const unsigned height) : Sorted(false), Current(false), Value(value) {
    Rect = new sf::RectangleShape(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    Rect->setFillColor(sf::Color::Green);
}

Bar::Bar(const Bar& bar) : Sorted(bar.Sorted), Current(bar.Current), Value(bar.Value) {
    if (bar.Rect) {
        this->Rect = new sf::RectangleShape(bar.Rect->getSize());
        this->Rect->setPosition(bar.Rect->getPosition());
        this->Rect->setFillColor(bar.Rect->getFillColor());
    }
}

Bar::Bar(Bar&& bar) : Sorted(bar.Sorted), Current(bar.Current), Value(bar.Value) { 
    this->Rect =  bar.Rect;
    bar.Rect = nullptr;
}

Bar& Bar::operator= (const Bar &bar) {
    // Check for self-assignment by comparing the address of the
    // implicit object and the parameter
    if (this == &bar)
        return *this;
 
    // Copies
    this->Sorted = bar.Sorted;
    this->Current = bar.Current;
    this->Value = bar.Value;
    if (this->Rect) {
        delete this->Rect;
        this->Rect = nullptr;
    }
    if (bar.Rect) {
        this->Rect = new sf::RectangleShape(bar.Rect->getSize());
        this->Rect->setPosition(bar.Rect->getPosition());
        this->Rect->setFillColor(bar.Rect->getFillColor());
    }
 
    return *this;
}

Bar::~Bar() {
    if (Rect) {
        delete Rect;
        Rect = nullptr;
    }
}

sf::Vector2f Bar::getPosition() const{
    return Rect->getPosition();
}

void Bar::draw(sf::RenderWindow* window) {
    window->draw(*Rect);
}

void Bar::setSorted() {
    if (Rect && !Sorted) 
        Sorted = true;
        Current = false;
        Rect->setFillColor(sf::Color::Red);
}

void Bar::setCurrent() {
    Sorted = false;
    Current = true;
    Rect->setFillColor(sf::Color::Magenta);
}

void Bar::setPosition(const sf::Vector2f& pos) {
    if (Rect)
        Rect->setPosition(pos);
}

void Bar::setPosition(const unsigned x, const unsigned y) {
    setPosition(sf::Vector2f(x, y));
}

void Bar::setUnsorted() {
    if (Sorted || Current) {
        Sorted = false;
        Current = false;
        Rect->setFillColor(sf::Color::Green);
    }
}
