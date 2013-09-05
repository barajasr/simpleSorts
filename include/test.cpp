#include <iostream>
#include <memory>
#include <vector>

using namespace std;

int maint(void) {
    vector<unique_ptr<unsigned>> Ints;
    for (unsigned i(0); i < 20; ++i) 
        Ints.emplace_back();
    return 0;
}
