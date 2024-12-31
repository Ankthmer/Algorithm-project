#ifndef __pandemia_h
#define __pandemia_h

#include "utilities.h"

typedef struct {
    uint n_individui;
    uint n_giorni;
    uint giorni_incubazione;
    uint giorni_convalescenza;
    uint letti_ospedale;
} Pandemia;

typedef struct {
    uint contagiati;
    uint ricoverati;
    uint rimasti;
    uint individui_in_attesa;
    uint giorni_degenza;
    uint giorni_quarantena;
    double giorni_in_attesa;
} Pandemia_stat;

/* leggi dal file i dati iniziali riguardanti popolazione e pandemia*/
void pandemia_new(FILE*, Pandemia*);

#endif
