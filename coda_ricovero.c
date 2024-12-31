#include "coda_ricovero.h"

/* Se (A[i+1], . . . , A[j]) è un heap confronta l’elemento A[i] con i suoi discendenti nell’albero ed esegue gli scambi opportuni in modo tale che anche il vettore (A[i], A[i+1], . . . , A[j]) rispetti la proprietà di un min heap.*/
void coda_ricovero_heapify(Individuo_array *coda_ricovero, uint i)
{
    uint k, j;

    j = coda_ricovero->lunghezza;

    /* Se la relazione seguente è soddisfatta */
    if ((2*i)+2 == j)
    {
        /* Verifico refrattarietà individui e in caso li scambio */
        if (confronta_refrattarieta(&coda_ricovero->p[i], &coda_ricovero->p[j-1]) > 0)
        {
            scambia_individui(&coda_ricovero->p[i],&coda_ricovero->p[j-1]);
        }
    }
    /* Altrimenti aggiorno il pivot */
    else if ((2*i)+2 < j)
    {
        if (confronta_refrattarieta(&coda_ricovero->p[(2*i)+1],&coda_ricovero->p[(2*i+2)]) < 0)
        {
            k = (2*i) + 1;
        }
        else
        {
            k = 2*i + 2;
        }
        /* Se indice e pivot non sono correttamente ordinati scambia gli individui ed esegue una chiamata ricorsiva */
        if (confronta_refrattarieta(&coda_ricovero->p[i],&coda_ricovero->p[k]) > 0)
        {
            scambia_individui(&coda_ricovero->p[i],&coda_ricovero->p[k]);
            coda_ricovero_heapify(coda_ricovero, k);
        }
    }
}

/* Aggiunge un elemento alla coda per il ricovero e poi la riorganizza in modo da mantenere la struttura di heap*/
void coda_ricovero_aggiungi (Individuo_array* coda_ricovero, Individuo* individuo)
{
    uint ultimo; /* Indica l'indice di inserimento */
    uint i; /* Variabile cicli for */

    ultimo = coda_ricovero->lunghezza;

    coda_ricovero->lunghezza++;

    /* Aggiungo l'individuo nel primo indice libero */
    coda_ricovero->p[ultimo] = *individuo;

    /* Se c'è più di un individuo in coda */
    if (coda_ricovero->lunghezza > 1)
    {
        /* Esegue un confronto di padre in padre per sistemare l'heap */
        for (i=(ultimo-1)/2; i < ultimo; i= (i-1)/2)
        {
            if (confronta_refrattarieta(&coda_ricovero->p[i],&coda_ricovero->p[ultimo])>0)
            {
                scambia_individui(&coda_ricovero->p[i],&coda_ricovero->p[ultimo]);
                ultimo = i;
            }
            else break;
        }
    }
}

/* Restituisce il primo individuo nella coda, lo elimina dalla coda e riorganizza la coda in modo da mantenere la struttura di heap. Prerequisito: la lunghezza della coda deve essere almeno di 1 */
Individuo coda_ricovero_prendi(Individuo_array *coda_ricovero)
{
    Individuo ricoverato;
    uint fine;

    assert(coda_ricovero->lunghezza > 0);

    /* Diminuisci di uno la lunghezza della coda */
    coda_ricovero->lunghezza--;

    /* Scambia il primo individuo con l'ultimo */
    fine = coda_ricovero->lunghezza;
    scambia_individui(&coda_ricovero->p[0], &coda_ricovero->p[fine]);
    /* Ricovera l'individuo all'ultimo posto */
    ricoverato = coda_ricovero->p[fine];
    /* Poichè dalla posizione 1 alla fine della coda rispetta la struttura di heap, chiama heapify per riordinare l'heap includendo la posizione 0 */
    coda_ricovero_heapify(coda_ricovero, 0);

    return ricoverato;
}
