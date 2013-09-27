#ifndef __BARS_HPP__
#define __BARS_HPP__

#include <vector>
#include <utility>

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
    void setRange(const unsigned start, const unsigned end, void (Bar::*state)());
	void siftDown(unsigned root, unsigned end);
    void swap(const unsigned ipos, const unsigned iipos);
    void visualSwap(const std::pair<unsigned, unsigned>& indices, void (Bar::*leftState)(), void (Bar::*rightState)());
    std::vector<Bar*> merge(std::vector<Bar*>& left, std::vector<Bar*>& right);
    std::vector<Bar*> mergeSort(const std::vector<Bar*>& array);

    public:
    Bars(sf::RenderWindow* root, unsigned numOfBars, unsigned height, unsigned width);
    ~Bars();
    bool isValid() const;
    void bubbleSort();
    void cocktailSort();
    void draw();
	void heapSort();
    void insertionSort();
    void mergeSort();
    void render();
    void reverse();
    void selectionSort();
    void shuffleBars();
};

#endif
