#ifndef __visita_grafo_h
#define __visita_grafo_h

#include "utilities.h"
#include "individuo_array.h"

typedef struct {
    uint *V;
    uint size; /* dimensione massima della coda */
    uint head; /* indice dell'ultima posizione occupata */
    uint tail; /* indice della prima posizione libera */
} Coda_contagio;

/* Calcola e stampa il numero massimo di giorni che ci vogliono per passare il contagio fra qualunque coppia di individui e il nome dei primi due individui in ordine alfabetico che rispettano questo numero di giorni */
void diametro_popolazione (Individuo_array*);

/* Visita in ampiezza il grafo G a partire dalla sorgente s marcando i vertici visitati in C con l'indice c */
void visita_grafo_ampiezza (Individuo_array*, uint, uint*);

/* Restituisce il massimo fra gli elementi di un vettore e aggiorna l'indice a cui si trova*/
uint max_giorni (uint *, Individuo_array*);


/* Crea una coda vuota */
void coda_contagio_new (Coda_contagio *, uint );

/* Elimina una coda esistente */
void coda_contagio_elimina (Coda_contagio *);

/* Controlla se la coda contiene elementi */
boolean coda_contagio_vuota_controllo (Coda_contagio *);

/* Restituisce la testa della coda */
uint coda_contagio_head (Coda_contagio *);

/* Inserisce un elemento in coda e sposta di uno l'indice della coda*/
void coda_contagio_enqueue (uint i, Coda_contagio *);

/* Sposta di uno l'indice della testa (eliminando di fatto il primo elemento) */
void coda_contagio_dequeue (Coda_contagio *);

#endif
