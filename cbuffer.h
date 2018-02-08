#ifndef CBUFFER_H
#define CBUFFER_H

#include <ostream>
#include <iostream>

/** \brief Buffer circolare
 * Implementa un buffer circolare di dimensione massima fissata contenente elementi di 
 * tipo generico.
 * Al riempimento del buffer, quando si inserisce un nuovo elemento, viene sovrascritto il più vecchio
 * @param T tipo del dato
 */
template <class T>
class cbuffer {
    /** \brief Struttura base per costruire la lista.
     * Contiene il dato generico di tipo T e il puntatore al container successivo.
     */
    struct container {
        T value; ///< Dato inserito nella lista
        container *next; ///> Puntatore al container successivo
        
        /** costruttore di default che inizializza a NULL */
        container() : next(0) {}
        /** \brief Costruttore che permette subito di inizializzare un nodo
         * con valori già esistenti
         */
        container(const T &value, container *next=NULL) : value(value), next(next) {}
    };

    /** \brief Puntatore alla testa del buffer */
    container *_head;
    /** \brief Puntatore alla coda del buffer */
    container *_tail;
    /** \brief Numero di elementi attualmente nel buffer */
    unsigned int _size;
    /** \brief Massimo numero di elementi contemporaneamente presenti nel buffer */
    unsigned int _max_size;

public:
    /** \brief Costruttore di default
     * Inizializza un buffer con dimensione massima a 10
     */
    cbuffer(unsigned int max=10) : _head(NULL), _tail(NULL), _size(0), _max_size(max) {}

    /** \brief Costruttore copia
     * 
     * @param other lista da copiare
     * @throw eccezione di fallita allocazione dinamica
     */
    cbuffer(const cbuffer &other) : _head(NULL), _tail(NULL), _size(0), _max_size(other._max_size) {
        const container *current = other._head;
        try {
            for (unsigned int i = 0; i < other._size; i++) {
                push_back(current->value);
                current = current->next;
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    /** \brief Costruttore a partire da una struttura dati iterabile 
     * Dati begin() e end() di una qualsiasi struttura dati, ne copia il contenuto in un buffer circolare
     * @param IT classe generica degli iteratori
     * @param begin iteratore di inizio
     * @param end iteratore di fine
     * @throw eccezione di fallita allocazione dinamica
     */
    template <class IT>
    cbuffer(unsigned int max, IT begin, IT end) : _max_size(max) {
        _head = NULL;
        _tail = NULL;
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

    /** \brief Rimuove un elemento dalla testa del buffer 
     * Se la lista non è vuota elimina l'elemento in testa deallocandolo
     */
    void pop() {
        if (_head != NULL) {
            container *old = _head;
            _head = _head->next;
            _size--;
            delete old;
            // Se ho eliminato l'unico elemento allora setto anche la coda a NULL
            if (_head == NULL)
                _tail = NULL;
        }
    }

    /** \brief Inserisce un elemento in coda
     * Se la lista è piena elimino l'elemento in testa e ne inserisco uno nuovo
     * Se la lista è vuota, faccio puntare _head e _tail al nuovo elemento creato
     * Altrimenti accodo semplicemente aggiornando il puntatore _tail
     * 
     * @param value Riferimento all'elemento di tipo T da inserire
     */
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
            _tail = temp;
            return;
        }
        // Collego l'ultimo nodo e aggiorno la coda
        _tail->next = temp;
        _tail = temp;
        _size++;
    }

    /** \brief Dealloca il buffer
     * Svuota completamente la lista deallocando ogni elemento
     */
    void clear() {
        container *temp;
        for (unsigned int i = 0; i < _size; i++) {
            temp = _head;
            _head = _head->next;
            delete temp;
        }
        _size = 0;
        _tail = NULL;
    }

    /** \brief Ritorna l'elemento in testa al buffer */
    T& top() const {
        if (_head == NULL)
            return NULL;
        return _head->value;
    }

    /** \brief Ritorna l'elemento in coda al buffer */
    T& tail() const {
        if (_tail == NULL)
            return NULL;
        return _tail->value;
    }

    /** \brief Numero di elementi presenti nel buffer */
    unsigned int size() {
        return _size;
    }

    /** \brief Numero massimo di elementi allocabili nel buffer */
    unsigned int capacity() {
        return _max_size;
    }

    /** \brief Operatore per accedere all'i-esimo elemento
     * Non è lineare, ma impiega O(n)
     * @param i posizione dell'elemento
     * @throw eccezione posizione non accessibile
     */
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

    /** \brief Operatore di assegnamento
     * @param other cbuffer da copiare
     * @return reference a this
     * @throw eccezione di memoria
     */
    cbuffer& operator=(const cbuffer &other) {
        if (this != &other) {
            cbuffer temp(other);
            std::swap(temp._head, _head);
            std::swap(temp._tail, _tail);
            std::swap(temp._size, _size);
            std::swap(temp._max_size, _max_size);
        }
        return *this;
    }

    /** \brief equals
     * Due cbuffer sono uguali se hanno la stessa dimensione fissa
     * lo stesso numero di elementi allocati e per ogni elemento
     * lo stesso valore
     * @param other lista da confrontare
     */
    bool equals(const cbuffer &other) const {
        if (other._size != _size || other._max_size != _max_size)
            return false;
        const container *current = _head;
        const container *c_other = other._head;
        for (unsigned int i = 0; i < _size; i++) {
            if (current->value != c_other->value)
                return false;
            current = current->next;
            c_other = c_other->next;
        }
        return true;
    }

    /** \brief operatore di disuguaglianza 
     * Effettua un not sull'operatore di uguaglianza
     * @param other lista da confrontare
     */
    bool operator!=(const cbuffer &other) const {
        return !(this == other);
    }

    /** \brief Distruttore che richiama clear() */
    ~cbuffer() {
        clear();
    }

    /** \brief iteratore di cbuffer (lettura e scrittura) */
    class iterator {
        private:
            /** \brief puntatore al container */
            container *current;
        public:
            iterator() : current(NULL) {}
            iterator(container *curr) : current(curr) {}
            iterator(const iterator &other) : current(other.current) {}

            ~iterator() {
                current = NULL;
            }

            /** \brief Dereferenziamento
             * Ritorna il dato riferito dall'iteratore 
             */
            T& operator*() const {
                return current->value;
            }

            /** \brief Ritorna il puntatore al dato di tipo T dell'elemento puntato */
            T* operator->() const {
                return &(current->value);
            }

            /** \brief Operatore di non uguaglianza 
             * @param other iteratore da confrontare
             * due iteratori sono diversi se:
             * - puntano a due cose diverse
             */
            bool operator!=(const iterator &other) const {
                return current != other.current;
            }

            /** \brief Operatore di uguaglianza 
             * @param other iteratore da confrontare
             * Richiama l'operatore != e lo nega
             */
            bool operator==(const iterator &other) const {
                return !(this != other);
            }

            /** \brief Operatore di assegnamento */
            iterator& operator=(const iterator &other) {
                current = other.current;
                return *this;
            }

            /** \brief Operatore di pre-incremento
             * Passa al container successivo, ritornando sè stesso
             */
            iterator& operator++() {
                current = current->next;
                return *this;
            }

            /** \brief Operatore di post-incremento
             * Passa al container successivo ma ritorna sè stesso prima dell'incremento
             */
            iterator operator++(int) {
                iterator it(*this);
                current = current->next;
                return it;
            }
    };

    /** \brief iteratore costante di cbuffer (sola lettura) */
    class const_iterator {
        private:
            /** \brief puntatore al container */
            const container *current;
        public:
            const_iterator() : current(NULL) {}
            const_iterator(container *curr) : current(curr) {}
            const_iterator(const const_iterator &other) : current(other.current) {}

            /** \brief Dereferenziamento
             * Ritorna il dato riferito dall'iteratore 
             */
            const T& operator*() const {
                return current->value;
            }

            /** \brief Ritorna il puntatore al dato di tipo T dell'elemento puntato */
            const T* operator->() const {
                return &(current->value);
            }

            /** \brief Operatore di non uguaglianza 
             * @param other iteratore da confrontare
             * due iteratori sono diversi se:
             * - puntano a due cose diverse
             */
            bool operator!=(const const_iterator &other) const {
                
                return current != other.current;
            }

            /** \brief Operatore di uguaglianza 
             * @param other iteratore da confrontare
             * Richiama l'operatore != e lo nega
             */
            bool operator==(const const_iterator &other) const {
                return !(this != other);
            }

            /** \brief Operatore di pre-incremento
             * Passa al container successivo, ritornando sè stesso
             */
            const_iterator& operator++() {
                current = current->next;
                return (*this);
            }

            /** \brief Operatore di assegnamento */
            const_iterator& operator=(const const_iterator &other) {
                current = other.current;
                return *this;
            }

            /** \brief Operatore di post-incremento
             * Setta _head_tail a false per indicare che abbiamo fatto una modifica
             * Passa al container successivo ma ritorna sè stesso prima dell'incremento
             */
            const_iterator operator++(int) {
                const_iterator it(*this);
                current = current->next;
                return it;
            }
    };

    /** \brief Iteratore dell'elemento in testa al buffer */
    iterator begin() {
        return iterator(_head);
    }

    /** \brief Iteratore che indica la fine del buffer */
    iterator end() {
        return iterator(0);
    }

    /** \brief Iteratore costante dell'elemento in testa al buffer */
    const_iterator begin() const {
        return const_iterator(_head);
    }

    /** \brief Iteratore costante che indica la fine del buffer */
    const_iterator end() const {
        return const_iterator(0);
    }
};

/** \brief Operatore di output 
 * Operando con gli iteratori scorre il buffer e ne visualizza i dati
 */
template <class T>
std::ostream &operator<<(std::ostream &os, const cbuffer<T> &cb) {
	
	typename cbuffer<T>::const_iterator i, ie;

	for(i = cb.begin(), ie = cb.end(); i!=ie; i++)
		os << *i << std::endl;

	return os;
}

/** \brief valuta il predicato unary_funct su ogni elemento di cb
 * @param cb cbuffer sul quale viene valutato il predicato
 * @param unary_funct funtore unario
 * Stampa a video il risultato di unary_funct per ogni elemento di cb 
 */
template <class T, class F>
void evaluate_if(const cbuffer<T> &cb, F unary_funct) {
    typename cbuffer<T>::const_iterator it = cb.begin();
    typename cbuffer<T>::const_iterator it_e = cb.end();
    for(int i = 0; it != it_e; it++, i++) {
        std::cout << i << ": " << (unary_funct(*it) ? "true" : "false") << std::endl;
    }
}

#endif