#include <algorithm> 
#include <climits>
#include <cstdlib>
#include <ctime>
#include <new>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../include/Bars.hpp"
#include "../include/Bar.hpp"

Bars::Bars(sf::RenderWindow* root, unsigned numOfBars, unsigned width, unsigned height)
    : scaleWidth(width/numOfBars -1), scaleHeight(height/numOfBars),
      window(root) {
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	if (root != nullptr) {
    	for (unsigned i(1); i <= numOfBars; ++i) {
            try{
    	        Collection.emplace_back(new Bar(i, scaleWidth, i*scaleHeight));
            } catch(const std::bad_alloc& exception) {
                Collection.clear();
                return;
            } 
    	    Collection.at(i-1)->setPosition({static_cast<float>((i-1)*(scaleWidth+1)),
    	                                     static_cast<float>(height-(i*scaleHeight))});
    	}
	}
}

Bars::~Bars() {
	for (unsigned i(0); i < Collection.size(); ++i) {
		if (Collection.at(i)){
			delete Collection.at(i);
			Collection.at(i) =  nullptr;
		}
	}
	window = nullptr; // Resource should be freed elsewhere
}

bool Bars::isValid() const {
    for (unsigned i(0); i < Collection.size(); ++i) 
        if (Collection.at(i)  == nullptr)
            return false;

    return true;
}

/**
	Stable
	O(1) extra space
	O(n²) comparisons and swaps
	Adaptive: O(n) when nearly sorted, with break checking for swaps
**/
void Bars::bubbleSort() {
	window->setTitle("Sort your shit: Bubble Sort");
	// Collection[0...i-1] are sorted
	unsigned size(Collection.size()-1);
	for (unsigned i(0); i <= size && window->isOpen(); ++i) {
		Bar* current = nullptr;
		for (unsigned j(size); j > i; --j) {
			this->checkForEvents();

			// Highlight current bar that will be used to compare left with
			current = Collection.at(j);
			current->setCurrent();
			current->draw(window);
			window->display();

			// Swap if needed, and update states
			if(current->getValue() < Collection.at(j-1)->getValue()) {
				this->visualSwap({j-1, j}, &Bar::setCurrent, &Bar::setUnsorted);
				window->display();
			} else {
				// No swap, reset, and  move on
				current->setUnsorted();
				current->draw(window);
			}
		}
		Collection.at(i)->setSorted();
		Collection.at(i)->draw(window);
	}
}

void Bars::checkForEvents() {
	sf::Event event;
	bool paused(false);
	while (window->pollEvent(event) || (paused && window->isOpen())) {
		if (event.type == sf::Event::Closed) {
            window->close();
	    } else if (event.type == sf::Event::KeyPressed) {
	    	if (event.key.code == sf::Keyboard::P)
	    		paused ^= true;
	    }
	    sf::sleep(sf::milliseconds(3));
	}
}

/**
	Stable
	O(1) extra space
	O(n²) comparisons and swaps
	Adaptive: O(n) when nearly sorted, with break checking for swaps
**/
void Bars::cocktailSort() {
	window->setTitle("Sort your shit: Cocktail Sort");
	// Collection[0...i-1] are sorted
    unsigned i{0}, size(Collection.size()-1);
    bool swapped{true};
	while(swapped && window->isOpen()) {
		Bar* current = nullptr;
        swapped = false;
        // Foward iteration
        for (unsigned j(i); j < size-i; ++j) {
			this->checkForEvents();

			// Highlight current bar that will be used to compare right with
			current = Collection.at(j);
			current->setCurrent();
			current->draw(window);
			window->display();

			// Swap if needed, and update states
			if(current->getValue() > Collection.at(j+1)->getValue()) {
                swapped = true;
				this->visualSwap({j, j+1}, &Bar::setUnsorted, &Bar::setCurrent);
				window->display();
			} else {
				// No swap, reset, and  move on
				current->setUnsorted();
				current->draw(window);
			}
		}
		Collection.at(size-i)->setSorted();
		Collection.at(size-i)->draw(window);
        window->display();

        if (swapped) {
            current = nullptr;
            for (unsigned j(size-i-1); j > i; --j) {
                this->checkForEvents();

                // Highlight current bar that will be used to compare left with
                current = Collection.at(j);
                current->setCurrent();
                current->draw(window);
                window->display();

                // Swap if needed, and update states
                if(current->getValue() < Collection.at(j-1)->getValue()) {
                    swapped = true;
                    this->visualSwap({j-1, j}, 
                            &Bar::setCurrent, &Bar::setUnsorted);
                    window->display();
                } else {
                    // No swap, reset, and  move on
                    current->setUnsorted();
                    current->draw(window);
                }
            }
            Collection.at(i)->setSorted();
            Collection.at(i)->draw(window);
            window->display();
        }
        ++i;
    }

    setRange(i-1, size-i+1, &Bar::setSorted);
}

void Bars::draw() {
    for (unsigned i(0); i < Collection.size(); ++i)
        Collection.at(i)->draw(window);
}

/**
	Stable
	O(1) extra space
	O(n²) comparisons and swaps
	Adaptive: O(n) time when nearly sorted
	Very low overhead
**/
void Bars::insertionSort() {
	window->setTitle("Sort your shit: Insertion Sort");
	// Collection[0...i-1] are sorted
	Bar* current = Collection.front();
	for (unsigned i(1); i < Collection.size() && window->isOpen(); ++i) {
	    for (unsigned j(i); j > 0 && (Collection.at(j)->getValue() < Collection.at(j-1)->getValue()); --j) {
			this->checkForEvents();

			// Swap needed
			current = Collection.at(j);
			this->visualSwap({j-1, j}, &Bar::setCurrent, &Bar::setSorted);
			window->display();
		}
		// Bar in sorted place
		if (current){
			current->setSorted();
			current->draw(window);
			window->display();
		}
	}
	// Bar needs to fill if tallest was left as unsorted on last iteration
	Collection.back()->setSorted();
	Collection.back()->draw(window);
}

std::vector<Bar*> Bars::merge(std::vector<Bar*>& left, std::vector<Bar*>& right) {
	std::vector<Bar*> result;
	while (left.size() > 0 || right.size() > 0){
		if (left.size() > 0 && right.size() > 0) {
			if (left.at(0)->getValue() <= right.at(0)->getValue()){
				result.emplace_back(left.at(0));
				left = std::vector<Bar*>(left.begin()+1, left.end());
			} else {
				result.emplace_back(right.at(0));
				right = std::vector<Bar*>(right.begin()+1, right.end());
			}
		} else if (left.size() > 0) {
			result.emplace_back(left.at(0));
			left = std::vector<Bar*>(left.begin()+1, left.end());
		} else if (right.size() > 0) {
			result.emplace_back(right.at(0));
			right = std::vector<Bar*>(right.begin()+1, right.end());
		}
	}

	// Sorted visuals
	unsigned leftMost(UINT_MAX);
	for (const auto& data : result){
		if (leftMost > data->getPosition().x)
			leftMost = data->getPosition().x;
	}
	for (auto& data : result) {
		auto pos = data->getPosition();
		pos.x = leftMost;
		data->setPosition(pos);
		leftMost += scaleWidth + 1;
	}
	// Draw sorted data over old set
	this->render();
	for (auto& data : result) {
		data->setSorted();
		data->draw(window);
	}
	window->display();

	return result;
}

/**
	Stable
	Θ(n) extra space for arrays (as shown)
	Θ(lg(n)) extra space for linked lists
	Θ(n·lg(n)) time
	Not adaptive
	Does not require random access to data
**/
void Bars::mergeSort() {
	window->setTitle("Sort your shit: Merge Sort");
	Collection = std::move(this->mergeSort(Collection));
}

std::vector<Bar*> Bars::mergeSort(const std::vector<Bar*>& array) {
	if (array.size() <= 1)
		return array;

	std::vector<Bar*> left, right;
	unsigned mid = (array.size()-1)/2;
	for (unsigned i(0); i <=mid; ++i) 
		left.emplace_back(array.at(i));
	for (unsigned i(mid+1); i < array.size(); ++i) 
		right.emplace_back(array.at(i));
	
	left = std::move(mergeSort(left));
	right = std::move(mergeSort(right));
	return merge(left, right);
}

void Bars::render() {
	if (window) {
		window->clear();
		this->draw();
		window->display();
	}
}

void Bars::reverse() {
	for (size_t start(0), end(Collection.size()-1); start < end; ++start, --end) {
		Collection.at(start)->setUnsorted();
		Collection.at(end)->setUnsorted();
		this->swap(start, end);
	}
}

/**
	Not stable
	O(1) extra space
	Θ(n²) comparisons
	Θ(n) swaps
	Not adaptive
**/
void Bars::selectionSort() {
	window->setTitle("Sort your shit: Selection Sort");
	// [0...i-1] sorted
	for (unsigned i(0), k(i); i < Collection.size() && window->isOpen(); ++i, k=i) {
	    Bar* current = nullptr;
	    // Scanning for smallest value in unsorted set
	    for (unsigned j(i+1); j < Collection.size(); ++j){
	    	this->checkForEvents();

	    	// Seen as scanning animation
	    	current = Collection.at(j);
	    	current->setCurrent();
	    	current->draw(window);
	    	window->display();
	    	current->setUnsorted();
	    	current->draw(window);

	    	if (current->getValue() < Collection.at(k)->getValue())
	    		k = j;
	    }

		// Swap if needed, set state to reflect change
	    current = Collection.at(i);
	    if (k!=i) {
		    this->visualSwap({i, k}, &Bar::setSorted, &Bar::setUnsorted);
    	} else {
    		current->setSorted();
    		current->draw(window);
    	}
	}
}

void Bars::setRange(const unsigned start, const unsigned end, void (Bar::*state)()) {
    if (start <= end && end < Collection.size())
        for (unsigned pos{start}; pos <= end; ++pos) {
            (Collection.at(pos)->*state)();
        }
}

void Bars::shuffleBars() {
	window->setTitle("Sort your shit");
	for (unsigned i(0); i < Collection.size(); ++i)
		this->swap(std::rand()%Collection.size(), std::rand()%Collection.size());
	for (auto& bar : Collection)
		bar->setUnsorted();
}

void Bars::swap(const unsigned ipos, const unsigned iipos) {
	unsigned size(Collection.size());
	if (ipos < size && iipos < size) {
		auto iSpritePos = Collection.at(ipos)->getPosition();
		auto iiSpritePos = Collection.at(iipos)->getPosition();

		Bar* tmp = Collection.at(ipos);
		Collection[ipos] = Collection.at(iipos);
		Collection.at(ipos)->setPosition(iSpritePos.x, iiSpritePos.y);
		Collection[iipos] =  tmp;
		Collection.at(iipos)->setPosition(iiSpritePos.x, iSpritePos.y);
	}
}

/**
	indices: indices of elements to be swapped, expecting smaller index first
	firstState, secondState: function pointers to: setBlank, setCurrent, setSorted, setUnsorted
							 Applied after swap, determines visual states on graph as viewed
							 left to right after swap.
	Visually blanks out bars found at indices, swaps them, and blits them with new state; no display().
**/
void Bars::visualSwap(const std::pair<unsigned, unsigned>& indices, void (Bar::*leftState)(), void (Bar::*rightState)()) {
	Bar* barOne = Collection.at(std::get<0>(indices));
	Bar* barTwo = Collection.at(std::get<1>(indices));
	barOne->setBlank();
	barOne->draw(window);
	barTwo->setBlank();
	barTwo->draw(window);
	this->swap(std::get<0>(indices), std::get<1>(indices));
	// Pointees flipped due to swap(), barTwo=left, barOne=right
	(barTwo->*leftState)();
	barTwo->draw(window);
	(barOne->*rightState)();
	barOne->draw(window);
}
