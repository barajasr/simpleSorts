#ifndef __BARS_HPP__
#define __BARS_HPP__

#include <vector>

namespace sf{
   class RenderWindow;
}

class Bar;

class Bars {
    private:
    const unsigned scaleWidth;
    const unsigned scaleHeight;
    std::vector<Bar*> Collection;
    sf::RenderWindow* window;

    void checkForEvents();
    void swap(unsigned ipos, unsigned iipos);
    std::vector<Bar*> merge(std::vector<Bar*>& left, std::vector<Bar*>& right);
    std::vector<Bar*> mergeSort(const std::vector<Bar*>& array);

    public:
    Bars(sf::RenderWindow* root, unsigned numOfBars, unsigned height, unsigned width);
    ~Bars();
    bool isValid() const;
    void bubbleSort();
    void draw();
    void insertionSort();
    void mergeSort();
    void render();
    void selectionSort();
    void shuffleBars();
};

#endif