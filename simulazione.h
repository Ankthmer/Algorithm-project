#ifndef __simulazione_h
#define __simulazione_h

#include "utilities.h"
#include "coda_ricovero.h"

/* Legge dal file passato da riga di comando i dati riguardanti la simulazione e li salva nelle apposite strutture dati */
void simulazione_new(FILE*, Pandemia*, Individuo_array*);

/* Stampa gli elenchi di individui ordinati in base all'età e in base alla refrattarietà */
void simulazione_stampa_elenchi(Individuo_array*);

/* Simula e stampa l'andamento del contagio per ogni individuo durante i giorni della durata della pandemia, e i relativi dati statistici. Legenda:
  - ’.’ per individui non infettivi (cioè sani, guariti o appena contagiati il giorno stesso)
  - ’I’ per individui infettivi e non rilevati
  - ’O’ per individui ricoverati in ospedale
  - ’Q’ per individui in quarantena domestica*/
void simulazione_stampa(Pandemia*, Individuo_array*);

/* Si dimettono gli individui sani dall'ospedale Se il numero di giorni trascorsi in quarantena più due volte quello dei giorni trascorsi in ospedale supera strettamente una determinata soglia un individuo viene dimesso o liberato dalla quarantena fin dalla mattina stessa */
uint dimissioni(Individuo_array*, Pandemia*, char*, uint);

/* Valuta quali individui devono essere inseriti all'interno della coda e li inserisce*/
void aggiorna_coda(Individuo_array*, Individuo_array*, Pandemia* , char*, uint);

/* Prende il primo individuo della coda, lo ricovera in ospedale e aggiorna il suo stato*/
boolean ricovera(Individuo_array*, Individuo_array*, char*, Pandemia*, Pandemia_stat*, uint, uint);

/* Riempie lo stato di ogni individuo del giorno dopo con uno stato provvisorio, indicando se ci sono nuovi individui contagiati */
void nuovi_contagi(Individuo_array*, char*, Pandemia_stat*, uint, uint);

/* Stampa le informazioni statistiche riguardanti la pandemia */
void stampa_statistica(char*, Individuo_array*, Pandemia*, Pandemia_stat* );

#endif
