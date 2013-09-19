#include <new>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "../include/Bar.hpp"

Bar::Bar() 
    : Blank(false), Current(false), Sorted(false), 
      Value(0), Rect(nullptr) {}

Bar::Bar(const unsigned value, const unsigned width, const unsigned height)
    : Blank(false), Current(false), Sorted(false), Value(value) {
    try {
        Rect = new sf::RectangleShape({static_cast<float>(width),
                                       static_cast<float>(height)});
    } catch (const std::bad_alloc& e) {
        Rect = nullptr;
        this->Value = 0;
        return;
    }
    Rect->setFillColor(sf::Color::Green);
}

Bar::Bar(const Bar& bar)
    : Blank(bar.Current), Current(bar.Current),
      Sorted(bar.Sorted), Value(bar.Value) {
    if (bar.Rect) {
        try {
            this->Rect = new sf::RectangleShape(bar.Rect->getSize());
        } catch (const std::bad_alloc& e) {
            this->Rect = nullptr;
            this->Value = 0;
            return;
        }
        this->Rect->setPosition(bar.Rect->getPosition());
        this->Rect->setFillColor(bar.Rect->getFillColor());
    }
}

Bar::Bar(Bar&& bar)
    : Blank(bar.Blank), Current(bar.Current),
      Sorted(bar.Sorted), Value(bar.Value) {
    this->Rect = bar.Rect;
    bar.Rect = nullptr;
}

Bar& Bar::operator= (const Bar &bar) {
    // Check for self-assignment
    if (this == &bar)
        return *this;
 
    // Copies
    this->Blank = bar.Blank;
    this->Current = bar.Current;
    this->Sorted = bar.Sorted;
    this->Value = bar.Value;
    if (this->Rect) {
        delete this->Rect;
        this->Rect = nullptr;
    }
    if (bar.Rect) {
        try {
            this->Rect = new sf::RectangleShape(bar.Rect->getSize());
        } catch(const std::bad_alloc& e) {
            this->Rect = nullptr;
            this->Value = 0;
            return *this;
        }
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
    if (Rect) {
        window->draw(*Rect);
    }
}

void Bar::setBlank() {
    if (!Blank || Current || Sorted) {
        Sorted = Current = false;
        Blank = true;
        Rect->setFillColor(sf::Color::Black);
    }
}

void Bar::setCurrent() {
    if (!Current || Blank || Sorted) {
        Sorted = Blank = false;
        Current = true;
        Rect->setFillColor(sf::Color::Magenta);
    }
}

void Bar::setSorted() {
    if (!Sorted || Blank || Current) { 
        Sorted = true;
        Blank = Current = false;
        Rect->setFillColor(sf::Color::Red);
    }
}

void Bar::setPosition(const sf::Vector2f& pos) {
    if (Rect)
        Rect->setPosition(pos);
}

void Bar::setPosition(const unsigned x, const unsigned y) {
    this->setPosition({static_cast<float>(x), static_cast<float>(y)});
}

void Bar::setUnsorted() {
    if (Sorted || Current || Blank) {
        Sorted = Current = Blank = false;
        Rect->setFillColor(sf::Color::Green);
    }
}
