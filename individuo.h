#ifndef __individuo_h
#define __individuo_h

#include "utilities.h"

typedef struct {
    uint id;
    double frequenza;
} Contatto;

typedef struct {
    Contatto *p;
    uint lunghezza;
} Contatto_array;

typedef struct {
    string nome;
    uint anni;
    double salute;
    uint giorni_contagio;
    uint numero_contatti;
    Contatto_array contatti;
} Individuo;

/* Legge dal file una riga di dati e crea un nuovo individuo senza dire chi sono i suoi contatti*/
void individuo_new(FILE *, Individuo *);

/* Confronta la refrattarietà fra due individui, restituisce -1 se il primo individuo ha un valore più basso, 0 se sono uguali e 1 se ha un valore più alto*/
int confronta_refrattarieta(Individuo*, Individuo*);

/* Calcola la refrattarità di un individuo */
double individuo_refrattarieta(Individuo*);

/* Libera la memoria dell'array contatti di un individuo */
void individuo_distruggi_contatti(Individuo*);

#endif
