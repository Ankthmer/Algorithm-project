#include "pandemia.h"

/* Leggi dal file i dati iniziali riguardanti popolazione e pandemia */
void pandemia_new(FILE *file, Pandemia *pandemia)
{
    uint n; /* Numero di variabili lette da fscanf */

    /* Legge la prima riga del file ed inserisce i dati in una struct di tipo Pandemia */
    n = fscanf(file,"%u %u %u %u %u",&pandemia->n_individui, &pandemia->n_giorni, &pandemia->giorni_incubazione, &pandemia->giorni_convalescenza, &pandemia->letti_ospedale);

    /* Controllo: fscanf deve leggere esattamente 5 parametri */
    assert(n == 5);
}
