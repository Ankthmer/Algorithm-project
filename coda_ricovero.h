#ifndef __coda_ricovero_h
#define __coda_ricovero_h

#include "utilities.h"
#include "individuo_array.h"

/* Confronta l’elemento A[i] con i suoi discendenti nell’albero ed esegue gli scambi opportuni in modo tale che anche il vettore
 (A[i], A[i+1], . . . , A[j]) rispetti la propriet`a dello heap.*/
void coda_ricovero_heapify(Individuo_array *, uint);

/* Aggiunge un elemento alla coda per il ricovero e poi la riorganizza in modo da mantenere la struttura di heap*/
void coda_ricovero_aggiungi (Individuo_array*, Individuo*);

/* Restituisce il primo individuo nella coda, lo elimina dalla coda e riorganizza la coda in modo da mantenere la struttura di heap */
Individuo coda_ricovero_prendi(Individuo_array*);

#endif
