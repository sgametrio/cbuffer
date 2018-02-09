# Relazione progetto `cbuffer`
Studente: Carrara Demetrio

Matricola: 807894

Indirizzo e-mail: d.carrara@campus.unimib.it

## Introduzione
Il progetto richiede di dover implementare un buffer circolare di dimensione fissata in fase di costruzione, contenente elementi di tipo generico `T`. La particolarità è il fatto che, una volta pieno, l'inserimento di un elemento in coda, elimina l'elemento più vecchio (ovvero l'elemento in testa)

## Scelte realizzative

### cbuffer
Ho scelto di utilizzare una lista di nodi `container` i quali puntano al nodo container successivo e contengono il valore dell'elemento di tipo `T`. Non ho utilizzato una lista circolare ma una semplice lista perchè la circolarità crea complessità nella gestione degli iteratori e non aggiunge niente di richiesto. Infatti per i nostri scopi, collegare `_head` e `_tail` (primo e ultimo elemento) non ci è di nessun aiuto. L'importante è gestire in modo corretto l'inserimento di nuovi elementi a buffer pieno simulando la circolarità (eliminando l'elemento più vecchio e inserendo in coda quello nuovo). Avrei potuto utilizzare un array ma la lista mi garantisce maggiore flessibilità sia nell'allocazione dinamica dei nodi `container` effettivamente occupati sia nell'eliminazione in testa e nell'inserimento in coda che sono immediati visto l'utilizzo di due puntatori `_head` e `_tail`.

In costruzione viene fissato l'attributo `_max_size` che rappresenta la massima capacità del buffer. Non viene allocata memoria per tutta la capacità del buffer, solo al momento dell'inserimento dell'elemento si ha un'effettiva allocazione dinamica. `_size` è il numero effettivo degli elementi attualmente all'interno del buffer. Il buffer è pieno quando `_size` è uguale a `_max_size`. Questi due attributi vengono dichiarati `unsigned` perchè non ha senso esprimere le dimensioni con un numero negativo.

La scelta di utilizzare due puntatori `_head` e `_tail` è dovuta al fatto di poter inserire e rimuovere in tempo costante un elemento.

E' possibile accedere agli elementi tramite operatori `[]` anche se l'accesso non è diretto e in tempo costante ma, essendo una lista, bisogna scorrere elemento per elemento fino all'i-esimo.


### container
E' la struct base che costituisce il nodo della lista e contiene:

* `value`: elemento di tipo `T`

* `next`: puntatore al prossimo nodo

Il suo compito è quello di salvare le informazioni base e di permettere lo scorrimento della lista tramite puntatori.


### iterator e const_iterator
`iterator` e `const_iterator` sono strutturati in modo simile ma con una differenza sostanziale. Accedendo agli elementi tramite `const_iterator` si garantisce la loro immutabilità (solo lettura), mentre tramite `iterator` è possibile accedere sia in lettura che in scrittura.

Vengono implementati come `forward_iterator` visto la natura sequenziale della lista, non a `random_access` come accade per gli array. 

Visto che la struttura base è una lista semplice, tramite i metodi `begin()` e `end()` restituisco rispettivamente gli iteratori all'elemento in testa e all'elemento successivo dell'ultimo cioè `NULL`. Ciclare sugli iteratori quindi diventa semplice, ad ogni incremento passo al nodo successivo tramite il puntatore `next` e una volta superato sull'ultimo nodo avrò l'iteratore nullo. 

### evaluate_if
Il predicato `evaluate_if` valuta un predicato unario F su ogni elemento del buffer cb di tipo generico e stampa per ogni elemento nel buffer la valutazione del predicato su di esso.

## Makefile

* `make docs`
    
    Genera la documentazione direttamente dai commenti del codice utilizzando il comando `doxygen Doxyfile`. Viene generato solo HTML, JS e CSS.

* `make clean`

    Rimuove i file oggetto, eseguibili e altri file che non servono. Pulisce cioè tutti i tipi di file che possono essere generati a partire dai file sorgente.

* `make test`

    Compila i sorgenti e la classe `cbuffer`. Esegue i test e ne stampa a video l'output.

* `make leak_check`

    Compila i sorgenti, ed esegue `valgrind` per verificare che non ci siano memory leaks.

* `make`

    Compila i sorgenti generando l'eseguibile `program`.
