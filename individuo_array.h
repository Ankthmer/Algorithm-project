#ifndef __individuo_array_h
#define __individuo_array_h

#include "utilities.h"
#include "pandemia.h"
#include "individuo.h"

typedef struct {
    Individuo *p;
    uint lunghezza;
} Individuo_array;


/* Leggi dal file i dati delle singole persone e crea un array con i loro dati */
void individuo_array_new(FILE*, Pandemia*, Individuo_array*);

/* Riordina l'array di individui in ordine alfabetico con l'algoritmo QuickSort */
void quick_sort_alfabetico (Individuo_array*, uint, uint);

/* Partiziona l'array di individui in due sottovettori sulla base dell'ordine alfabetico*/
uint partition_alfabetico (Individuo_array*, uint, uint);

/* Scambia il posto di due individui all'interno dell'array */
void scambia_individui (Individuo*, Individuo*);

/* Associa ad ogni individuo i suoi contatti */
void individuo_array_associa_contatti(FILE*, Individuo_array*, Pandemia*);

/* Restituisce l'indice del punto dell'array dove si trova l'individuo */
uint individuo_array_ricerca(string, Individuo_array*);

/* Crea l'array di contatti di un individuo */
Contatto_array contatto_array_new(FILE*, uint, Individuo_array*);

/* Crea una copia di un array di individui */
Individuo_array individuo_array_copia(Individuo_array*);

/* Riordina l'array di individui in ordine d'età con l'algoritmo QuickSort */
void quick_sort_anni (Individuo_array*, uint, uint);

/* Partiziona l'array di individui in due sottovettori sulla base dell'ordine d'età */
uint partition_anni (Individuo_array*, uint, uint);

/* Stampa gli individui in ordine per età */
void individuo_array_stampa_ordine_anni (Individuo_array*);

/* Riordina l'array di individui in ordine di refrattarietà con l'algoritmo QuickSort  */
void quick_sort_refrattarieta (Individuo_array*, uint, uint);

/* Partiziona l'array di individui in due sottovettori sulla base dell'ordine d'età */
uint partition_refrattarieta (Individuo_array*, uint, uint);

/* Stampa gli individui in ordine per refrattarietà */
void individuo_array_stampa_ordine_refrattarieta (Individuo_array*);

/* Distrugge un array di individui senza toccare i contatti */
void individuo_array_distruggi_copia(Individuo_array*);

/* Distrugge un array di individui */
void individuo_array_distruggi(Individuo_array*);

#endif
