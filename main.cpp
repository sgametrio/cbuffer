#include "cbuffer.h"
#include <iostream>
#include <cassert>
#include <cstddef>

template <class T>
struct is_zero{
    bool operator()(const T &a) const {
        return a == 0;
    }
};

int test_array[3] = {1, 0, 3};
cbuffer<int> test_cb(3, test_array, test_array+3);

char test_array2[5] = {'A', 'E', 'I', 'O', 'U'};
cbuffer<char> test_cb2(5, test_array2, test_array2+5);

void test_push_less_n() {
    std::cout << "Test inserimento elementi <= max_size: ";
    cbuffer<int> cb(3);
    cb.push_back(21);
    cb.push_back(0);
    cb.push_back(55);
    bool passed = !cb.equals(test_cb);
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_push_more_n() {
    std::cout << "Test inserimento elementi > max_size: ";
    cbuffer<char> cb(5);
    cb.push_back('W');
    cb.push_back('E');
    cb.push_back('A');
    cb.push_back('E');
    cb.push_back('I');
    cb.push_back('O');
    cb.push_back('U');
    bool passed = cb.equals(test_cb2);
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_operator_equal() {
    std::cout << "Test operatore = : ";
    cbuffer<int> cb(3);
    cb = test_cb;
    bool passed = cb.equals(test_cb);
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_copy_constructor() {
    std::cout << "Test costruttore copia: ";
    cbuffer<int> cb(test_cb);
    bool passed = cb.equals(test_cb);
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_direct_access() {
    std::cout << "Test accesso diretto con operatore []: ";
    int array[4] = {2, 3, 4, 1};
    cbuffer<int> cb(4, array, array+4);
    int first = cb[0];
    int last = cb[3];
    cb.push_back(7);
    int new_first = cb[0];
    int new_last = cb[3];
    bool passed = 
        first == 2 &&
        last == 1 &&
        new_first == 3 &&
        new_last == 7;
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_output() {
    std::cout << "Output cbuffer pieno: " << std::endl;
    int array[4] = {2, 3, 4, 1};
    cbuffer<int> cb(4, array, array+4);
    cbuffer<int> cb_empty(3);
    std::cout << cb;
    std::cout << "Output cbuffer vuoto: " << std::endl;
    std::cout << cb_empty;
}

void test_modify_element() {
    std::cout << "Test modifica elemento [] e iteratore: ";
    int array[4] = {2, 3, 4, 1};
    cbuffer<int> cb(4, array, array+4);
    cb[2] = 7;
    array[2] = 7;
    *cb.begin() = 5;
    array[0] = 5;
    cbuffer<int> cb_a(4, array, array+4);
    std::cout << (cb.equals(cb_a) ? "PASSED" : "FAILED") << std::endl;
}

void test_creazione_cb_da_cb() {
    std::cout << "Test creazione buffer da un altro buffer pieno: ";
    cbuffer<char> cb_s(4);
    cb_s.push_back('A');
    cb_s.push_back('2');
    cb_s.push_back(66);
    cb_s.push_back('$');
    cbuffer<char>::iterator it, it_e;
    it = cb_s.begin();
    it_e = cb_s.end();
    const cbuffer<char> cb_s2(4, it, it_e);
    cbuffer<char>::const_iterator c_it, c_it_e;
    c_it = cb_s2.begin();
    c_it_e = cb_s2.end();
    cbuffer<char> cb_s3(4, c_it, c_it_e);
    std::cout << (cb_s.equals(cb_s2) && cb_s2.equals(cb_s3) ? "PASSED" : "FAILED") << std::endl;
}

void test_pop() {
    std::cout << "Test pop elemento: ";

}

void test_evaluate_if() {
    std::cout << "evaluate_if con funtore is_zero: " << std::endl;
    evaluate_if(test_cb, is_zero<int>());
}

int main() {
    test_push_less_n();
    test_push_more_n();
    test_direct_access();
    test_modify_element();
    test_creazione_cb_da_cb();
    test_operator_equal();
    test_copy_constructor();
    test_output();
    test_evaluate_if();
    return 0;
}