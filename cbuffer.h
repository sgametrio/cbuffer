#ifndef CBUFFER_H
#define CBUFFER_H

#include <ostream>

template <class T>
class cbuffer {
    // Componiamo il nodo container della lista
    struct container {
        T value;
        container *next;
        
        container() : next(0) {}
        container(const T &value, container *next=NULL) : value(value), next(next) {}
    };

    container *_head;
    unsigned int _size;
    unsigned int _max_size;

public:
    cbuffer(unsigned int max=10) : _head(NULL), _size(0), _max_size(max) {}
    template <class IT>
    cbuffer(unsigned int max, IT begin, IT end) : _max_size(max) {
        _head = NULL;
        _size = 0;
        try {
            for(; begin != end; begin++) {
                push_back(static_cast<T>(*begin));
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    // Rimuovi un elemento in testa
    void pop() {
        if (_head != NULL) {
            container *old = _head;
            _head = _head->next;
            _size--;
            delete old;
        }
    }

    // Inserisci un elemento in coda
    void push_back(const T &value) {
        // Se il buffer è pieno, elimino l'elemento in testa e inserisco di nuovo
        if (_head != NULL && _size == _max_size) {
            pop();
            push_back(value);
            return;
        }

        container *temp = new container(value);
        if (_head == NULL) {
            _head = temp;
            _size++;
            return;
        }
        container *last = _head;
        // Scorro fino in fondo per accodare il nodo in ultima posizione
        for(unsigned int i = 1; i < _size; i++) {
            last = last->next;
        }
        last->next = temp;
        _size++;
        // Se ho appena riempito il buffer aggiorno il puntatore dell'ultimo alla testa
        if(_size == _max_size) {
            temp->next = _head;
        }
    }

    void clear() {
        container *temp;
        for (unsigned int i = 0; i < _size; i++) {
            temp = _head;
            _head = _head->next;
            delete temp;
        }
        _size = 0;
    }

    T& operator[](unsigned int i) {
        if (i >= _size) {
            throw("Posizione non istanziata o non accessibile");
        }
        container *current = _head;
        for (unsigned int j = 0; j < i; j++) {
            current = current->next;
        }
        return current->value;
    }

    ~cbuffer() {
        clear();
    }

    class iterator {
        private:
            container *current;
            bool _head_tail;
        public:
            iterator() : current(NULL), _head_tail(true) {}
            iterator(container *curr) : current(curr), _head_tail(true) {}
            iterator(const iterator &other) : current(other.current), _head_tail(other._head_tail) {}

            // Ritorna il dato riferito dall'iteratore (dereferenziamento)
            T& operator*() const {
                return current->value;
            }

            // Ritorna il puntatore al dato riferito dall'iteratore
            T* operator->() const {
                return &(current->value);
            }

            bool operator!=(const iterator &other) const {
                return _head_tail || current != other.current;
            }

            bool operator==(const iterator &other) const {
                return !(this != other);
            }
            // pre-incremento
            iterator& operator++() {
                _head_tail = false;
                current = current->next;
                return (*this);
            }

            // post-incremento
            iterator& operator++(int) {
                iterator it(*this);
                _head_tail = false;
                current = current->next;
                return it;
            }
    };

    iterator begin() {
        return iterator(_head);
    }

    iterator end() {
        // devo dire in tempo costante l'iteratore, non posso permettermi di ciclare tutto
        if (_size == _max_size) {
            // lista circolare -> end() == begin()
            return iterator(_head);
        } else {
            return iterator(0);
        }
    }
};

template <class T>
std::ostream &operator<<(std::ostream &os, 
	cbuffer<T> &cb) {
	
	typename cbuffer<T>::iterator i, ie;

	for(i = cb.begin(), ie = cb.end(); i!=ie; ++i)
		os << *i << std::endl;

	return os;
}

#endif