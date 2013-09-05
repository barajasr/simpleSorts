#ifndef __BAR_HPP__
#define __BAR_HPP__

#include <vector>

#include <SFML/System/Vector2.hpp>

namespace sf{
    class RectangleShape;
    class RenderWindow;
}

class Bar {
    private:
    bool Blank;
    bool Current;
    bool Sorted;
    unsigned Value;
    sf::RectangleShape* Rect;

    public:
    Bar();
    Bar(const unsigned value, const unsigned width, const unsigned height);
    Bar(const Bar& bar);
    Bar(Bar&& bar);
    Bar& operator= (const Bar& bar);
    ~Bar();
    sf::Vector2f getPosition() const;
    unsigned getValue() const { return Value;}
    void draw(sf::RenderWindow* window);
    void setBlank();
    void setSorted();
    void setCurrent();
    void setPosition(const sf::Vector2f& pos);
    void setPosition(const unsigned x, const unsigned y);
    void setUnsorted();
};

#endif

