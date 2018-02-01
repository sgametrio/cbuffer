#include "cbuffer.h"
#include <iostream>
#include <cassert>

bool test_push_less_n() {
    try {
        cbuffer<int> cb(5);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        // TODO con operatore ==
        return true;
    } catch (...) {
        throw;
    }
    
}

bool test_push_more_n() {
    try {
        cbuffer<int> cb(5);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);
        cb.push_back(4);
        cb.push_back(5);
        cb.push_back(6);
        cb.push_back(7);
        // TODO con operatore ==
        return true;
    } catch (...) {
        throw;
    }
    
}

bool test_iterator_constructor() {
    try {
        int array[3] = {2, 3, 4};
        cbuffer<int> cb(3, array, array+3);
        // TODO con operatore ==
        return true;
    } catch (...) {
        throw;
    }
    
}

bool test_direct_access() {
    int array[4] = {2, 3, 4, 1};
    cbuffer<int> cb(4, array, array+4);
    int first = cb[0];
    int last = cb[3];
    cb.push_back(7);
    int new_first = cb[0];
    int new_last = cb[3];
    return first == 2 &&
            last == 1 &&
            new_first == 3 &&
            new_last == 7;
}

void test_output() {
    int array[4] = {2, 3, 4, 1};
    cbuffer<int> cb(4, array, array+4);
    std::cout << cb;
}

int main() {
    assert(test_push_less_n() && "Can't push less objects than max size");
    assert(test_push_more_n() && "Error in deleting old and pushing new objects");
    assert(test_iterator_constructor() && "Can't instantiate a cbuffer with iterators");
    assert(test_direct_access() && "direct access not working correctly");
    test_output();
    return 0;
}