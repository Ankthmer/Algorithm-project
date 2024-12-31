#include "individuo.h"

/* Legge dal file una riga di dati e crea un nuovo individuo senza dire chi sono i suoi contatti */
void individuo_new(FILE *file, Individuo *individuo)
{
    uint n; /* Numero di variabili lette da fscanf */

    /* Legge la riga fino al valore che indica il numero dei contatti di un individuo */
    n = fscanf(file, "%s %u %lg %u %u", individuo->nome, &individuo->anni, &individuo->salute, &individuo->giorni_contagio, &individuo->numero_contatti);

    /* Controllo: fscanf deve leggere esattamente 5 parametri */
    assert(n == 5);

    /* Scarta il resto della riga */
    while (fgetc(file) != '\n');
}

/* Confronta la refrattarietà fra due individui, restituisce -1 se il primo individuo ha un valore più basso, 0 se sono lo stesso individui e 1 se ha un valore più alto*/
int confronta_refrattarieta(Individuo* i, Individuo* j)
{
    int confronto;
    double ra,rb;
    ra=individuo_refrattarieta(i);
    rb=individuo_refrattarieta(j);
    confronto = (ra > rb)-(ra < rb);

    if (confronto == 0)
    {
      confronto = strcmp(i->nome, j->nome);
    }

    return confronto;
}

/* Calcola la refrattarità di un individuo */
double individuo_refrattarieta(Individuo *individuo)
{
    double refrattarita;

    refrattarita = individuo->salute*pow(1-((double)individuo->anni/100), 3);

    return refrattarita;
}

/* Libera la memoria dell'array contatti di un individuo */
void individuo_distruggi_contatti(Individuo* individuo)
{
    free(individuo->contatti.p);
    individuo->contatti.p = NULL;
    individuo->contatti.lunghezza = 0;
}
