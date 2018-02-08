# Relazione progetto `cbuffer`
Studente: Carrara Demetrio

Matricola: 807894

Indirizzo e-mail: d.carrara@campus.unimib.it

## Introduzione
Il progetto richiede di dover implementare un buffer circolare di dimensione fissata in fase di costruzione, contenente elementi di tipo generico `T`. La particolarità è il fatto che, una volta pieno, l'inserimento di un elemento in coda, elimina l'elemento più vecchio (ovvero l'elemento in testa)

## Scelte realizzative

### cbuffer
Ho scelto di utilizzare una lista circolare di nodi `container` i quali puntano al nodo container successivo e contengono il valore dell'elemento di tipo `T`. In caso di buffer pieno, essendo una lista circolare, il nodo più recente avrà un riferimento al successivo (quello più vecchio). Avrei potuto utilizzare un array ma la lista mi garantisce maggiore flessibilità sia nell'allocazione dinamica dei nodi `container` effettivamente occupati sia nell'eliminazione in testa e nell'inserimento in coda che sono immediati visto l'utilizzo di due puntatori `_head` e `_tail`.

In costruzione viene fissato l'attributo `_max_size` che rappresenta la massima capacità del buffer. Non viene allocata memoria per tutta la capacità del buffer, solo al momento dell'inserimento dell'elemento effettivo per non sprecare memoria. `_size` è il numero effettivo degli elementi attualmente all'interno del buffer. Il buffer è pieno quando `_size` è uguale a `_max_size`. Questi due attributi vengono dichiarati `unsigned` perchè non ha senso esprimere le dimensioni con un numero negativo.

La scelta di utilizzare due puntatori `_head` e `_tail` è dovuta al fatto di poter inserire e rimuovere in tempo costante un elemento e poter restituire gli iteratori `begin()` e `end()` in tempo costante senza dover ciclare dalla testa all'ultimo elemento

E' possibile accedere agli elementi tramite operatori `[]` anche se l'accesso non è diretto e in tempo costante ma, essendo una lista, bisogna scorrere elemento per elemento fino all'i-esimo.


### container
E' la struct base che costituisce il nodo della lista e contiene:
* `value`: elemento di tipo `T`
* `next`: puntatore al prossimo nodo

Il suo compito è quello di salvare le informazioni base e di permettere lo scorrimento della lista tramite puntatori.


### iterator e const_iterator
`iterator` e `const_iterator` sono strutturati in modo simile ma con una differenza sostanziale. Accedendo agli elementi tramite `const_iterator` si garantisce la loro immutabilità (solo lettura), mentre tramite `iterator` è possibile accedere sia in lettura che in scrittura.

Vengono implementati come `forward_iterator` visto la natura sequenziale della lista, non a `random_access` come accade per gli array. 

Se il buffer è pieno, allora l'elemento successivo a quello in coda punta all'elemento in testa e non a NULL. Nascono quindi dei problemi per la gestione di `begin()`, `end()` e gli operatori di uguaglianza. Visto che gli iteratori sono nati proprio per scorrere in modo naturale una struttra dati, e per essere confrontati fra di loro si avrà subito che il begin() equivale all'end() (se il buffer è pieno) perchè puntano entrambi allo stesso elemento. Per ovviare a questo problema ho introdotto una variabile booleana che indica se l'iteratore ha già avuto una modifica oppure è la prima volta che viene modificato. Nel caso non avesse mai subito una modifica `end()` sarà diverso da `begin()`. Appena viene incrementato (pre o post) il confronto diventa "reale" e il ciclo terminerà quando si arriverà in fondo.

### evaluate_if
Il predicato `evaluate_if` valuta un predicato unario F su ogni elemento del buffer cb di tipo generico e stampa per ogni elemento nel buffer la valutazione del predicato su di esso.

## Makefile

